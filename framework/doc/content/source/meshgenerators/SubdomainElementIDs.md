# SubdomainElementIDs

!syntax description /Mesh/SubdomainElementIDs

## Overview

The `SubdomainElementIDs` assigns element IDs based on mesh subdomain IDs of an input mesh.
If the element IDs do not exist in the input mesh, they will be added by this mesh generator.
Default element IDs can be provided for subdomains not to be assigned with new IDs.
Otherwise, if the element IDs have existed in the mesh, their IDs will not be changed; or if the element IDs are new, invalid IDs are set to elements of those subdomains not to be assigned with new IDs.

!syntax parameters /Mesh/SubdomainElementIDs

!syntax inputs /Mesh/SubdomainElementIDs

!syntax children /Mesh/SubdomainElementIDs
