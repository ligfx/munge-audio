@lexer = [
[:string, "Track"], [:lparen], [:string, "Loopy"], [:rparen],
[:lbrace],
  [:string, "AleotoricLayer"], [:lparen], [:string, "Poooosh"], [:rparen],
  [:lbrace],
    [:string, "Volume"], [:lparen], [:number, 5.6], [:rparen],
    [:string, "Update"],
    [:lbrace],
      [:string, "someVariable"], [:operator, "="],
        [:string, "Add"], [:lparen], [:number, 6], [:comma], [:number, 7], [:rparen],
    [:rbrace],
    [:string, "Voice"],
    [:lbrace],
      [:string, "Wave"], [:lparen], [:string, "Bob"], [:rparen],
    [:rbrace],
  [:rbrace],
[:rbrace],
[:eoi]
]

# Test "expected function"
# @lexer = [[:lparen]]
# @lexer = [[:string, "Update"], [:lbrace], [:number, 6], [:rbrace]]

# Test "expected argument list, block, or operator"
# @lexer = [[:string, "Add"], [:rparen]]

# Test "expected comma between arguments"
# @lexer = [[:string, "Add"], [:lparen], [:number, 6], [:number, 7], [:rparen]]

# Test "expected number, variable name, or function"
# @lexer = [[:string, "Add"], [:operator, "="], [:comma]] 

def expected (s)
  puts "Expected #{s}, got #{@lexer.first[0]}"
  exit
end

def parse_string # Expectation handled above
  return false if @lexer.first[0] != :string
  puts "String: " + @lexer.first[1]
  @lexer.shift # string
  
  return true
end

def parse_number # Expectation handled above
  return false if @lexer.first[0] != :number
  puts "Number: " + @lexer.first[1].to_s
  @lexer.shift # number
  
  return true
end

def parse_comma # Expectation handled above
  return false if @lexer.first != [:comma]
  @lexer.shift # comma
  
  return true
end

def parse_operator # Expectation handled above
  return false if @lexer.first[0] != :operator
  puts "Operator: " + @lexer.first[1]
  @lexer.shift # operator
  
  return true
end

def parse_arg
  # An arg can be a string, a number, or a  function (without a block, but that's
  # a bit hard to test)
  return parse_number if @lexer.first[0] == :number # Don't need expectation
  
  # Problem: Both strings and functions start with strings =/
  # One solution: have method, parse_function_without_name (name)
  if @lexer.first[0] == :string
    return false if !parse_string # Don't need expectation
    # If it looks like a function after the string, then it's a function!
    # Note: The lexer needs to define a token called :operator
    if [:lparen, :lbrace, :operator].include? @lexer.first[0]
      return parse_function_without_name # Expectation handled inside
    end
    # Otherwise, it's just a name, and we're good
    return true
  end
  
  # Not a string (/function) or number? Woops!
  expected "number, variable name, or function"
end

# This method parses a function that has already had its name parsed (needed by
# parse_arg)
def parse_function_without_name
  # Now we can parse args, args block, block, or operator arg
  # If it starts with a parentheses, it's an arglist
  function = false
  if @lexer.first == [:lparen]
    @lexer.shift # lparen
    function = true
    while @lexer.first != [:rparen]
      return false if !parse_arg # Expectation handled inside
      if @lexer.first != [:rparen]
        expected "comma between arguments" if !parse_comma
      end
    end
    @lexer.shift # rparen
  end
  # If it starts with a curly brace, it's a block
  if @lexer.first == [:lbrace]
    @lexer.shift # lbrace
    function = true
    while @lexer.first != [:rbrace]
      return false if !parse_function # Expectation handled inside
    end
    @lexer.shift #rbrace
  end
  
  # If it didn't have either args or a block, then it must be an operand
  if !function
    expected "argument list, block, or operator" if !parse_operator
    return if !parse_arg # Expectation handled inside
  end
  
  return true
end

def parse_function
  # Parse name
  expected "function" if !parse_string
  # Parse body
  return if !parse_function_without_name # Expectation handled inside
  
  return true
end

def parse_toplevel
  while @lexer.first != [:eoi]
    return false if !parse_function
  end
  @lexer.shift # eoi
  
  return true
end

puts parse_toplevel
