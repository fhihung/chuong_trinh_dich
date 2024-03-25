import re

def json_lexer(text):
    tokens = []
    for match in re.finditer(r"(?<!\\)(?:\"[^\"]*(?:\\.|.)*\"|\[|\]|\{|\}|:\s*|,\s*|(?:\d+|[a-zA-Z_]+))", text):
        token = match.group(0)  # Use group(0) instead of group(1)
        if token in ["true", "false", "null"]:
            tokens.append((token, "keyword"))
        elif token in ["[", "]", "{", "}", ":", ","]:
            tokens.append((token, "punctuation"))
        elif re.match(r"\d+", token):
            tokens.append((token, "number"))
        else:
            tokens.append((token, "identifier"))
    return tokens

def test_lexer(json_file):
    with open(json_file, "r") as f:
        json_text = f.read()

    lexer = json_lexer(json_text)
    for token, kind in lexer:
        print(f"{token} ({kind})")


# Ví dụ sử dụng
text = """
{
    "name": "John Doe",
    "age": 30,
    "address": 
    {
        "street": "123 Main Street",
        "city": "New York",
        "state": "NY",
        "zip": 10001
    },
    "hobbies": 
    [
        "programming",
        "reading",
        "hiking"
    ]
}
"""

# Parse json qua string
tokens = json_lexer(text)
for token, kind in tokens:
    print(f"{token} ({kind})")

# Parse json qua file
tokenize = test_lexer("test.json")
