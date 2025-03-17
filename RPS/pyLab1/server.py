from xmlrpc.server import SimpleXMLRPCServer
from str_modul import *

server = SimpleXMLRPCServer(("localhost", 9000))
print("Listening on port 9000...")
server.register_function(number_of_words, "number_of_words")
server.register_function(is_palindrome, "is_palindrome")
server.register_function(remove_duplicates, "remove_duplicates")
server.register_function(capitalize_words, "capitalize_words")
server.serve_forever()