import torch
try:
    import torchvision
except ImportError as e:
    pass

from ._torch_blade import *

import torch_blade.utils as utils
import torch_blade.tools as tools
import torch_blade.version as version

from torch_blade.config import Config
import torch_blade.optimize as optimize
import warnings
try:
    import torch_blade.pai_internal
except ImportError as e:
    pass

try:
    import torch_blade.mlir
except ImportError as e:
    pass

warnings.filterwarnings("ignore")

utils.add_method(torch._C.ScriptModule)(tools.create_method_from_graph)
utils.add_method(torch._C.ScriptModule)(tools.unsafe_remove_method)
utils.add_method(torch._C.ScriptModule, "register_attribute")(tools.register_attr)
utils.add_method(torch._C.Graph, "createGetAttr")(tools.graph_create_get_attr)
torch._C.Graph.node_list = utils.listify(torch._C.Graph.nodes)
torch._C.Graph.input_list = utils.listify(torch._C.Graph.inputs)
torch._C.Graph.output_list = utils.listify(torch._C.Graph.outputs)
torch._C.Block.node_list = utils.listify(torch._C.Block.nodes)
torch._C.Node.input_list = utils.listify(torch._C.Node.inputs)
torch._C.Node.output_list = utils.listify(torch._C.Node.outputs)
utils.add_method(torch._C.Node,
                 "control_deps")(utils.find_control_dependencies)
utils.add_method(torch._C.Node, "isBefore")(tools.node_is_before)
utils.add_method(torch._C.Node, "isAfter")(tools.node_is_after)
