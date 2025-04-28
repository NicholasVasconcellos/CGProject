# CGAL-Based Face Clustering for Parallel Computation

## Overview
This project explores clustering faces in a 2D triangulation based on edge orientation, aiming to identify groups of independent cells that could be computed in parallel. It uses CGAL's 2D Triangulation structures and custom clustering algorithms to efficiently build face groups without introducing high computational overhead.

## Key Concepts
- **Triangulation**: Built using CGAL's `Triangulation_2` class with a custom face base to store cluster information.
- **Clustering Criteria**: Faces are grouped if the connecting edge is approximately vertical, within a specified angular tolerance.
- **Traversal Strategy**: A BFS/DFS traversal ensures O(N) complexity by avoiding expensive cluster merges.

## Features
- Custom `Cluster` and `Clusters` structs to manage groups of faces.
- Supports several types of point distributions for analysis:
  - Uniform random distribution
  - Clustered points (higher density regions)
  - Uniform grid
  - Grid with offsets

## How It Works
1. **Triangulation Generation**: Create a 2D triangulation from random or deterministic point sets.
2. **Clustering**: Traverse adjacent faces based on edge angle to build clusters.
3. **Visualization and Statistics**: Generate a histogram of cluster sizes for different point set types.

## Usage
Code and examples are available here:  
👉 [GitHub Repository](https://github.com/NicholasVasconcellos/CGProject)

## Future Extensions
- Expand clustering to Delaunay triangulations.
- Extend methods to 3D triangulations.
- Integrate coloring by cluster in visualizations.
