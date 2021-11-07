# CoarseMeshElementID

!syntax description /Mesh/CoarseMeshElementID

## Overview

This mesh generator add an element integer of coarse element IDs to an input mesh based on a coarse mesh.
Grffin finds the coarse element where the centroid of an element on the input mesh (fine mesh) is located and assign the coarse element ID of the fine element with the coarse element ID.
Users can optionally check if the fine mesh is embedded in the coarse mesh, i.e. every element on the fine mesh must be contained
with one and only one coarse element of the coarse mesh.

!syntax parameters /Mesh/CoarseMeshElementID

!syntax inputs /Mesh/CoarseMeshElementID

!syntax children /Mesh/CoarseMeshElementID
