import ast
import graphviz

class ASTVisualizer(ast.NodeVisitor):
    def __init__(self):
        self.graph = graphviz.Digraph(format='png')
        self.graph.attr(rankdir='TB', ranksep='0.5', nodesep='0.5')
        self.graph.attr('node', fontname='helvetica', fontsize='12', height='0.4', width='1.0')
        self.graph.attr('edge', penwidth='0.7', arrowsize='0.9')

    def add_node(self, node, label, **attrs):
        node_id = str(id(node))
        all_attrs = {'shape': 'box', 'style': 'filled,rounded', 'fillcolor': '#ffffff'}
        all_attrs.update(attrs)
        self.graph.node(node_id, label, **all_attrs)
        return node_id

    def visit_Module(self, node):
        module_id = self.add_node(node, 'Module', fillcolor='#b4a7d6')
        for i, body_item in enumerate(node.body):
            child_id = self.visit(body_item)
            self.graph.edge(module_id, child_id, f'body[{i}]')

    def visit_FunctionDef(self, node):
        func_id = self.add_node(node, f'FunctionDef\nname: {node.name}', fillcolor='#b4a7d6')
        
        args_id = self.add_node(node.args, 'arguments', fillcolor='#f9cb9c')
        self.graph.edge(func_id, args_id, 'args')
        
        for i, arg in enumerate(node.args.args):
            arg_id = self.visit(arg)
            self.graph.edge(args_id, arg_id, f'args[{i}]')
        
        for i, body_item in enumerate(node.body):
            child_id = self.visit(body_item)
            self.graph.edge(func_id, child_id, f'body[{i}]')
        
        return func_id

    def visit_If(self, node):
        if_id = self.add_node(node, 'If', fillcolor='#a2c4c9')
        
        test_id = self.visit(node.test)
        self.graph.edge(if_id, test_id, 'test')
        
        for i, body_item in enumerate(node.body):
            child_id = self.visit(body_item)
            self.graph.edge(if_id, child_id, f'body[{i}]')
        
        for i, orelse_item in enumerate(node.orelse):
            child_id = self.visit(orelse_item)
            self.graph.edge(if_id, child_id, f'orelse[{i}]')
        
        return if_id

    def visit_BoolOp(self, node):
        bool_op_id = self.add_node(node, 'BoolOp', fillcolor='#a2c4c9')
        op_id = self.add_node(node.op, type(node.op).__name__, fillcolor='#ffe599')
        self.graph.edge(bool_op_id, op_id, 'op')
        
        for i, value in enumerate(node.values):
            value_id = self.visit(value)
            self.graph.edge(bool_op_id, value_id, f'values[{i}]')
        
        return bool_op_id

    def visit_Compare(self, node):
        compare_id = self.add_node(node, 'Compare', fillcolor='#a2c4c9')
        
        left_id = self.visit(node.left) 
        self.graph.edge(compare_id, left_id, 'left')
        
        for i, (op, comparator) in enumerate(zip(node.ops, node.comparators)):
            op_id = self.add_node(op, type(op).__name__, fillcolor='#ffe599')
            self.graph.edge(compare_id, op_id, f'ops[{i}]')
            
            comparator_id = self.visit(comparator)
            self.graph.edge(compare_id, comparator_id, f'comparators[{i}]')
        
        return compare_id

    def visit_BinOp(self, node):
        bin_op_id = self.add_node(node, 'BinOp', fillcolor='#a2c4c9')
        
        left_id = self.visit(node.left)
        self.graph.edge(bin_op_id, left_id, 'left')
        
        op_id = self.add_node(node.op, type(node.op).__name__, fillcolor='#ffe599')
        self.graph.edge(bin_op_id, op_id, 'op')
        
        right_id = self.visit(node.right)
        self.graph.edge(bin_op_id, right_id, 'right')
        
        return bin_op_id

    def visit_Name(self, node):
        return self.add_node(node, f'Name\nid: {node.id}', fillcolor='#f9cb9c')

    def visit_Num(self, node):
        return self.add_node(node, f'Num\nn: {node.n}', fillcolor='#f9cb9c')

    def visit_Load(self, node):
        return self.add_node(node, 'Load', fillcolor='#f9cb9c')

    def visit_Return(self, node):
        return_id = self.add_node(node, 'Return', fillcolor='#b4a7d6')
        value_id = self.visit(node.value)
        self.graph.edge(return_id, value_id, 'value')
        return return_id

    def visit_Str(self, node):
        return self.add_node(node, f'Str\ns: {node.s}', fillcolor='#f9cb9c')

    def visit_Call(self, node):
        call_id = self.add_node(node, 'Call', fillcolor='#a2c4c9')
        func_id = self.visit(node.func)
        self.graph.edge(call_id, func_id, 'func')
        for i, arg in enumerate(node.args):
            arg_id = self.visit(arg)
            self.graph.edge(call_id, arg_id, f'args[{i}]')
        return call_id

    def visit_arg(self, node):
        return self.add_node(node, f'arg\narg: {node.arg}', fillcolor='#f9cb9c')

    def visualize(self, tree):
        self.visit(tree)
        return self.graph

# FizzBuzz function
code = """
def fizzbuzz(n):
    if n % 3 == 0 and n % 5 == 0:
        return "FizzBuzz"
    elif n % 3 == 0:
        return "Fizz"
    elif n % 5 == 0:
        return "Buzz"
    else:
        return str(n)
"""

# Parse the Python code into an AST
tree = ast.parse(code)

# Create an AST visualizer instance
visualizer = ASTVisualizer()

# Generate and render the visualization
graph = visualizer.visualize(tree)
graph.render('fizzbuzz_ast', view=True)