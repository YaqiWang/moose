//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SubdomainElementIDs.h"

registerMooseObject("MooseApp", SubdomainElementIDs);

#include "libmesh/elem.h"

InputParameters
SubdomainElementIDs::validParams()
{
  InputParameters params = MeshGenerator::validParams();
  params.addRequiredParam<MeshGeneratorName>(
      "input", "Name of an existing mesh generator to which we assign element IDs");
  params.addRequiredParam<std::vector<SubdomainName>>("subdomains",
                                                      "Subdomain names present in the input mesh");
  params.addRequiredParam<std::vector<std::string>>("extra_id_names",
                                                    "List of user-defined extra element ID names");
  params.addParam<std::vector<std::vector<dof_id_type>>>(
      "extra_ids",
      "User-defined extra element IDs corresponding to 'subdomains' in the same order");

  params.addParam<std::vector<dof_id_type>>(
      "default_extra_ids", "Default extra element IDs for elements not in 'subdomains'");

  params.addClassDescription("Assign extra IDs for elements on a mesh based on mesh subdomains.");
  return params;
}

SubdomainElementIDs::SubdomainElementIDs(const InputParameters & params)
  : MeshGenerator(params),
    _input(getMesh("input")),
    _subdomain_names(getParam<std::vector<SubdomainName>>("subdomains"))
{
  if (_subdomain_names.size() == 0)
    paramError("subdomains", "Empty subdomain vector provided!");
}

std::unique_ptr<MeshBase>
SubdomainElementIDs::generate()
{
  std::unique_ptr<MeshBase> mesh = std::move(_input);

  // construct a map from the subdomain ID to the index in 'subdomain'
  std::map<SubdomainID, unsigned int> subdomains;
  for (unsigned int i = 0; i < _subdomain_names.size(); ++i)
  {
    SubdomainID id = Moose::INVALID_BLOCK_ID;
    std::istringstream ss(_subdomain_names[i]);

    if (!(ss >> id) || !ss.eof())
      id = mesh->get_id_by_name(_subdomain_names[i]);

    auto check = subdomains.insert(std::make_pair(id, i));
    if (!check.second)
      paramError("subdomains", "Cannot have subdomain with ID ", id, " listed more than once!");
  }

  auto & extra_id_names = getParam<std::vector<std::string>>("extra_id_names");
  auto & extra_ids = getParam<std::vector<std::vector<dof_id_type>>>("extra_ids");

  if (extra_id_names.size() != extra_ids.size())
    paramError("extra_ids", "Inconsistent vector size for extra IDs");
  for (auto & extra_id : extra_ids)
  {
    if (_subdomain_names.size() != extra_id.size())
      paramError("extra_ids", "Inconsistent vector size for extra IDs");
  }

  std::vector<unsigned int> xid;
  for (auto & extra_id_name : extra_id_names)
  {
    if (!mesh->has_elem_integer(extra_id_name))
      xid.push_back(mesh->add_elem_integer(extra_id_name));
    else
      xid.push_back(mesh->get_elem_integer_index(extra_id_name));
  }

  if (isParamValid("default_extra_ids"))
  {
    auto & default_ids = getParam<std::vector<dof_id_type>>("default_extra_ids");
    if (default_ids.size() != extra_id_names.size())
      paramError("default_extra_ids", "Inconsistent vector size for default extra IDs");

    for (auto & elem : mesh->element_ptr_range())
    {
      for (unsigned int i = 0; i < extra_ids.size(); ++i)
        elem->set_extra_integer(xid[i], default_ids[i]);
    }
  }

  for (auto & elem : mesh->element_ptr_range())
  {
    SubdomainID id = elem->subdomain_id();
    auto it = subdomains.find(id);
    if (it == subdomains.end())
      continue;

    for (unsigned int i = 0; i < extra_ids.size(); ++i)
      elem->set_extra_integer(xid[i], extra_ids[i][it->second]);
  }

  return mesh;
}
