# CoarseElementIDFixup

!syntax description /Mesh/CoarseElementIDFixup

## Overview

This mesh generator reassigns element IDs for a mesh such that elements with the same element ID, referred as coarse element later, form a connected region through sides of these fine elements.
This mesh generator only attempts fixup or reassignment of element IDs when all individual coarse elements are not completely disconnected, i.e. connected through mesh nodes at least.
When performing fixups, this mesh generator founds the nodes of a coarse element that in-between disconnected regions of the coarse element as illustrated in [fig:coarse-id-fixup] with the green node
and reassign all fine elements connecting to the nodes (the two elements with ID 172 and 213 next to the isolated fine element in [fig:coarse-id-fixup]) with the coarse element ID.

!media figures/coarse_element_id_fixup.png id=fig:coarse-id-fixup style=width:25%; caption=A coarse element that requires fixup

Doing so can make the coarse element have a single connected region.
This mesh generator also check the coarse elements that lose fine elements due to ID reassignments to make sure that they are still forming a single connected region.
The algorithm implemented in this mesh generator is not trying to find the minimum number of ID reassignements to meet the goal.
This is especially the case for three-dimensional meshes where fine elements in a coarse element may be disconnected through sides but connected through line segments.

This mesh generator also merges coarse eleements with single fine elements into their neighboring coarse elements.
The element ID reassignment can be viewed with the screen print-out when the [!param](/Mesh/CoarseElementIDFixup/verbose) is set to true.

!syntax parameters /Mesh/CoarseElementIDFixup

!syntax inputs /Mesh/CoarseElementIDFixup

!syntax children /Mesh/CoarseElementIDFixup
