import graphviz
import json
import os
import uuid
import subprocess
import tempfile

def nodeid() -> str:
  return str(uuid.uuid4())

def addnode(g: graphviz.Graph, node: dict):
  assert isinstance(node, dict)
  assert len(node) == 1
  name = next(iter(node.keys()))
  items = node[name] or []
  assert isinstance(items, list)

  parent = nodeid()
  g.node(parent, name, fontname="Fira Code Medium", fontsize="18")

  for item in items:
    if isinstance(item, str):
      child = nodeid()
      g.node(child, item)
    else:
      child = addnode(g, item)
    g.edge(parent, child)
  
  return parent

def main():
  expression = input("expression: ")
  output = subprocess.check_output(["go", "run", "."], input=expression.encode())

  d = json.loads(output)
  g = graphviz.Graph("", node_attr={
    "fontname": "Fira Code",
    "fontsize": "14"
  })

  addnode(g, d)

  tmpdir = tempfile.mkdtemp()
  g.render("out", tmpdir, format="png")
  print(f"rendered to {tmpdir}/out.png")

  os.system(f"open {tmpdir}/out.png")  

if __name__ == '__main__':
  main()