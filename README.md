# Nutri-lang

![example](ntri-example.png)

1. 📚 **Language Specification**

   - Syntax and Grammar:
     - Variables: `var x = 10;`
     - Arithmetic operations: `+`, `-`, `*`, `/`
     - Comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`
     - Control flow: `if`, `else`, `while`, `for`
     - Functions: `func add(a, b) { return a + b; }`
   - Data Types: `int`, `float`, `bool`, `string`

2. 🧩 **Lexical structure** — (recursive descent parser for simplicity)

   - Lexical structure:
     - Keywords: `var`, `if`, `else`, `while`, `for`, `func`, `return`
     - Identifiers: Alphanumeric characters and underscores, starting with a letter or underscore.
     - Literals: Integers, floating-point numbers, booleans (`true`, `false`), strings (enclosed in double quotes).
     - Operators: `+`, `-`, `*`, `/`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `=`
     - Delimiters: `(`, `)`, `{`, `}`, `;`
