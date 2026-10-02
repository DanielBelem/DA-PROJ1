## T1: Basic Application Setup & Data Parsing (4.0 points)

- [X]  **T1.1:** Develop a command-line menu to access all functionalities.
- [X]  **T1.2:** Read and parse the CSV input data.
- [X]  **T1.3:** Provide proper documentation (using Doxygen) and analyze the time complexity of key algorithms.

## T2: Route Planning for Driving (8.0 points)

### T2.1 (3.0 points):
  - [X] Compute the fastest driving route between a given source and destination.
    - Shortest Path (Dijkstra)
  - [X] Identify an alternative independent route (no overlapping nodes/segments except the endpoints).
    - Remove the nodes and segments from graph and use Dijkstra again

### T2.2 (5.0 points):
  - [X] Implement restricted route planning by allowing the exclusion of specific nodes and/or segments or enforcing the inclusion of a specific node.
    - Create a subgraph without the nodes and/or segments
    - If it enforces the inclusion of a specific node:
      - Shortest path source -> enforced node + shortest path enforced node -> sink

## T3: Environmentally-Friendly Route Planning (6.0 points)
### T3.1 (4.0 points):
  - [X] Plan routes that combine driving and walking.
  - [X] Ensure the route includes at least one driving segment and one walking segment.
  - [ ] Respect the maximum walking time specified by the user; if multiple routes have the same total time, select the one with the longest walking segment.

### T3.2 (2.0 points):
  - [X] When no suitable route is found, display alternative suggestions with approximate adjustments.

## T4: Demo & Presentation (2.0 points)

- [ ] Prepare a **10-minute demo** highlighting:
    - The functionality of the developed tool.
    - Key aspects and modifications of the graph data structure.
- [X] Provide a supporting **PowerPoint presentation (PDF)**.
