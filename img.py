
from collections import namedtuple
import sys
import os
from PIL import Image, ImageDraw

Node = namedtuple('Node', ['id', 'level'])
Connection = namedtuple('Connection', ['inumber', 'enabled', 'weight', 'node_in', 'node_out'])


def read_data(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        lines = [line.strip() for line in f]

    def to_node(s):
        node_id, node_level = s.split()
        return Node(int(node_id), int(node_level[1:-1]))


    inputs = []
    hidden = []
    outputs = []
    connections = []
    connection_started = False
    for line in lines:
        if not connection_started:
            if line.startswith('inputs = '):
                inputs = [to_node(v.strip()) for v in line[9:].split(',')]
            elif line.startswith('hidden = '):
                hidden = [to_node(v.strip()) for v in line[9:].split(',')]
            elif line.startswith('outputs = '):
                outputs = [to_node(v.strip()) for v in line[10:].split(',')]
            elif line == 'connections:':
                connection_started = True
        else:
            parts = line.split()
            if len(parts) == 5:
                inumber, enabled, weight, node_in, node_out = parts
                conn = Connection(int(inumber), enabled == 'on', float(weight), int(node_in), int(node_out))
                connections.append(conn)
    levels = set()
    for node in inputs + hidden + outputs:
        levels.add(node.level)
    return sorted(levels), inputs, hidden, outputs, connections


def render(data):
    levels, inputs, hidden, outputs, connections = data
    all_nodes = inputs + hidden + outputs
    node_levels = []
    max_width = 0
    for level in levels:
        nodes = [node for node in all_nodes if node.level == level]
        max_width = max(max_width, len(nodes))
        node_levels.append(nodes)

    boxw = boxh = 50
    size = (boxw*max_width, boxh*len(node_levels))
    im = Image.new('RGB', size, (255, 255, 255))
    draw = ImageDraw.Draw(im)

    print(size)
    r = 5
    middle_x = (boxw*max_width) / 2
    for y_base, nodes in enumerate(node_levels):
        y = (y_base * boxh) + boxh/2
        for x_relbase, node in enumerate(nodes):
            x_rel = -((boxw/2) * len(nodes))
            x_base = x_rel + boxw * x_relbase
            x = x_base + boxw/2
            x += middle_x
            print(x, y)
            draw.ellipse([x-r, y-r, x+r, y+r], fill=(0, 0, 0))

    im.show()

if __name__ == '__main__':
    data = read_data(sys.argv[1])
    render(data)
