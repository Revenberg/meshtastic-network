#!/bin/bash

# Map waar de gegenereerde bestanden worden opgeslagen
OUTPUT_DIR="esp-config/nodes"

# Zorg dat de output map bestaat
mkdir -p $OUTPUT_DIR

# Genereer bestanden voor node1, node2, en node3
for node in node1 node2 node3; do
  sed "s/node_id_placeholder/$node/g" esp-config/nodes/node_template.yaml > "$OUTPUT_DIR/$node.yaml"
done

echo "Node configuraties gegenereerd in $OUTPUT_DIR"