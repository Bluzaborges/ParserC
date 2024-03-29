Analisador sintático do subconjunto da linguagem C
============================

### Créditos

Trabalho discente efetivo da disciplina de linguagens formais.  
Professor: Ricardo Vargas Dorneles

Produzido por Bruno Borges Luza

### Instruções

O programa a ser analisado deve ser colocado no arquivo fonte.c.



* * *

### Gramática

Gramática produzida para analisador sintático do subconjunto da linguagem C. A gramática encontra-se fatorada e sem recursões à esquerda. Referência: [www.lysator.liu.se/c/ANSI-C-grammar-y.html](https://www.lysator.liu.se/c/ANSI-C-grammar-y.html).  

<pre>

<a name="START">START</a>
    : <a href="#EXTERNAL_DECLARATION">EXTERNAL_DECLARATION</a> <a href="#START_LIST">START_LIST</a>
    ;

<a name="START_LIST">START_LIST</a>
    : <a href="#EXTERNAL_DECLARATION">EXTERNAL_DECLARATION</a> <a href="#START_LIST">START_LIST</a>
    | &epsilon; 
    ;

<a name="EXTERNAL_DECLARATION">EXTERNAL_DECLARATION</a>
    : <a href="#TYPE">TYPE</a> <a href="#DECLARATOR">DECLARATOR</a> <a href="#EXTERNAL_DECLARATION_">EXTERNAL_DECLARATION_</a>
    ;

<a name="EXTERNAL_DECLARATION_">EXTERNAL_DECLARATION_</a>
    : <a href="#COMPOUND_BLOCK">COMPOUND_BLOCK</a>
    | <a href="#DECLARATION">DECLARATION</a>
    ;

<a name="TYPE">TYPE</a>
    : unsigned <a href="#TYPE_NAME">TYPE_NAME</a>
    | <a href="#TYPE_NAME">TYPE_NAME</a>
    ;

<a name="TYPE_NAME">TYPE_NAME</a>
    : int
    | long <a href="#LONG_DECLARATION">LONG_DECLARATION</a>
    | float
    | char
    | double
    ;

<a name="LONG_DECLARATION">LONG_DECLARATION</a>
    : int
    | long int
    ;

<a name="DECLARATION">DECLARATION</a>
    : <a href="#INIT_DECLARATOR">INIT_DECLARATOR</a> <a href="#INIT_DECLARATOR_LIST">INIT_DECLARATOR_LIST</a> ;
    ;

<a name="INIT_DECLARATOR_LIST">INIT_DECLARATOR_LIST</a>
    : , <a href="#DECLARATOR">DECLARATOR</a> <a href="#INIT_DECLARATOR">INIT_DECLARATOR</a> <a href="#INIT_DECLARATOR_LIST">INIT_DECLARATOR_LIST</a>
    | &epsilon;
    ;

<a name="INIT_DECLARATOR">INIT_DECLARATOR</a>
    : = <a href="#ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a>
    | &epsilon;
    ;

<a name="DECLARATOR">DECLARATOR</a>
    : * <a href="#DIRECT_DECLARATOR">DIRECT_DECLARATOR</a>
    | <a href="#DIRECT_DECLARATOR">DIRECT_DECLARATOR</a>
    | &epsilon;
    ;

<a name="DIRECT_DECLARATOR">DIRECT_DECLARATOR</a>
    : identifier <a href="#DIRECT_DECLARATOR_">DIRECT_DECLARATOR_</a>
    ;

<a name="DIRECT_DECLARATOR_">DIRECT_DECLARATOR_</a>
    : ( <a href="#DIRECT_DECLARATOR_PARENTHESIS">DIRECT_DECLARATOR_PARENTHESIS</a>
    | [ <a href="#DIRECT_DECLARATOR_BRACKET">DIRECT_DECLARATOR_BRACKET</a>
    | &epsilon;
    ;

<a name="DIRECT_DECLARATOR_PARENTHESIS">DIRECT_DECLARATOR_PARENTHESIS</a>
    : <a href="#PARAMETER_LIST">PARAMETER_LIST</a> )  <a href="#DIRECT_DECLARATOR_">DIRECT_DECLARATOR_</a>
    | <a href="#IDENTIFIER_LIST">IDENTIFIER_LIST</a> )  <a href="#DIRECT_DECLARATOR_">DIRECT_DECLARATOR_</a>
    | ) <a href="#DIRECT_DECLARATOR_">DIRECT_DECLARATOR_</a>
    ;

<a name="DIRECT_DECLARATOR_BRACKET">DIRECT_DECLARATOR_BRACKET</a>
    : constant ] <a href="#DIRECT_DECLARATOR_">DIRECT_DECLARATOR_</a>
    | ] <a href="#DIRECT_DECLARATOR_">DIRECT_DECLARATOR_</a>
    ;

<a name="PARAMETER_LIST">PARAMETER_LIST</a>
    : <a href="#PARAMETER">PARAMETER</a> <a href="#PARAMETER_LIST_">PARAMETER_LIST_</a>
    | &epsilon;
    ;

<a name="PARAMETER_LIST_">PARAMETER_LIST_</a>
    : , <a href="#PARAMETER">PARAMETER</a> <a href="#PARAMETER_LIST_">PARAMETER_LIST_</a>
    | &epsilon;
    ;

<a name="PARAMETER">PARAMETER</a>
    : <a href="#TYPE">TYPE</a> <a href="#PARAMETER_IDENTIFIER">PARAMETER_IDENTIFIER</a>
    ;

<a name="PARAMETER_IDENTIFIER">PARAMETER_IDENTIFIER</a>
    : * identifier
    | identifier
    ; 

<a name="IDENTIFIER_LIST">IDENTIFIER_LIST</a>
    : identifier <a href="#IDENTIFIER_LIST">IDENTIFIER_LIST</a>
    | &epsilon;
    ; 

<a name="COMPOUND_BLOCK">COMPOUND_BLOCK</a>
    : { <a href="#COMPOUND_BLOCK_CORE">COMPOUND_BLOCK_CORE</a>
    ; 

<a name="COMPOUND_BLOCK_CORE">COMPOUND_BLOCK_CORE</a>
    : }
    | <a href="#STATEMENT">STATEMENT</a> <a href="#STATEMENT_LIST">STATEMENT_LIST</a> }
    ; 

<a name="STATEMENT_LIST">STATEMENT_LIST</a>
    : <a href="#STATEMENT">STATEMENT</a> <a href="#STATEMENT_LIST">STATEMENT_LIST</a>
    | &epsilon;
    ; 

<a name="STATEMENT">STATEMENT</a>
    : <a href="#TYPE">TYPE</a> <a href="#DECLARATOR">DECLARATOR</a> <a href="#DECLARATION">DECLARATION</a>
    | ;
    | <a href="#EXPRESSION">EXPRESSION</a> ;
    | if ( <a href="#EXPRESSION">EXPRESSION</a> ) <a href="#COMPOUND_BLOCK">COMPOUND_BLOCK</a> <a href="#ELSE_STATEMENT">ELSE_STATEMENT</a>
    | while ( <a href="#EXPRESSION">EXPRESSION</a> ) <a href="#COMPOUND_BLOCK">COMPOUND_BLOCK</a>
    | do <a href="#COMPOUND_BLOCK">COMPOUND_BLOCK</a> while ( <a href="#EXPRESSION">EXPRESSION</a> ) ;
    | for ( <a href="#EXPRESSION_STATEMENT">EXPRESSION_STATEMENT</a> <a href="#EXPRESSION_STATEMENT">EXPRESSION_STATEMENT</a> <a href="#FOR_ENCLOSED">FOR_ENCLOSED</a>
    | continue ;
    | break ;
    | return <a href="#EXPRESSION_STATEMENT">EXPRESSION_STATEMENT</a>
    ;

<a name="ELSE_STATEMENT">ELSE_STATEMENT</a>
    : else <a href="#COMPOUND_BLOCK">COMPOUND_BLOCK</a>
    | &epsilon;
    ; 

<a name="FOR_ENCLOSED">FOR_ENCLOSED</a>
    : ) <a href="#COMPOUND_BLOCK">COMPOUND_BLOCK</a>
    | <a href="#EXPRESSION">EXPRESSION</a> ) <a href="#COMPOUND_BLOCK">COMPOUND_BLOCK</a>
    ;

<a name="EXPRESSION_STATEMENT">EXPRESSION_STATEMENT</a>
    : ;
    | <a href="#EXPRESSION">EXPRESSION</a> ;
    ;

<a name="EXPRESSION">EXPRESSION</a>
    : <a href="#ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a> <a href="#EXPRESSION_">EXPRESSION_</a>
    ;

<a name="EXPRESSION_">EXPRESSION_</a>
    : , <a href="#ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a> <a href="#EXPRESSION_">EXPRESSION_</a>
    | &epsilon;
    ; 

<a name="ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a>
    : <a href="#LOGICAL_OR_EXPRESSION">LOGICAL_OR_EXPRESSION</a> <a href="#ASSIGNMENT_EXPRESSION_">ASSIGNMENT_EXPRESSION_</a>
    ;

<a name="ASSIGNMENT_EXPRESSION_">ASSIGNMENT_EXPRESSION_</a>
    : = <a href="#ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a>
    | += <a href="#ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a>
    | -= <a href="#ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a>
    | *= <a href="#ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a>
    | /= <a href="#ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a>
    | %= <a href="#ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a>
    | &epsilon;
    ; 

<a name="LOGICAL_OR_EXPRESSION">LOGICAL_OR_EXPRESSION</a>
    : <a href="#LOGICAL_AND_EXPRESSION">LOGICAL_AND_EXPRESSION</a> <a href="#LOGICAL_OR_EXPRESSION_">LOGICAL_OR_EXPRESSION_</a>
    ;

<a name="LOGICAL_OR_EXPRESSION_">LOGICAL_OR_EXPRESSION_</a>
    : || <a href="#LOGICAL_AND_EXPRESSION">LOGICAL_AND_EXPRESSION</a> <a href="#LOGICAL_OR_EXPRESSION_">LOGICAL_OR_EXPRESSION_</a>
    | &epsilon;
    ; 

<a name="LOGICAL_AND_EXPRESSION">LOGICAL_AND_EXPRESSION</a>
    : <a href="#BITWISE_OR_EXPRESSION">BITWISE_OR_EXPRESSION</a> <a href="#LOGICAL_AND_EXPRESSION_">LOGICAL_AND_EXPRESSION_</a>
    ;

<a name="LOGICAL_AND_EXPRESSION_">LOGICAL_AND_EXPRESSION_</a>
    : && <a href="#BITWISE_OR_EXPRESSION">BITWISE_OR_EXPRESSION</a> <a href="#LOGICAL_AND_EXPRESSION_">LOGICAL_AND_EXPRESSION_</a>
    | &epsilon;
    ; 

<a name="BITWISE_OR_EXPRESSION">BITWISE_OR_EXPRESSION</a>
    : <a href="#BITWISE_XOR_EXPRESSION">BITWISE_XOR_EXPRESSION</a> <a href="#BITWISE_OR_EXPRESSION_">BITWISE_OR_EXPRESSION_</a>
    ;

<a name="LOGICAL_AND_EXPRESSION_">BITWISE_OR_EXPRESSION_</a>
    : | <a href="#BITWISE_XOR_EXPRESSION">BITWISE_XOR_EXPRESSION</a> <a href="#BITWISE_OR_EXPRESSION_">BITWISE_OR_EXPRESSION_</a>
    | &epsilon;
    ; 

<a name="BITWISE_XOR_EXPRESSION">BITWISE_XOR_EXPRESSION</a>
    : <a href="#BITWISE_AND_EXPRESSION">BITWISE_AND_EXPRESSION</a> <a href="#BITWISE_XOR_EXPRESSION_">BITWISE_XOR_EXPRESSION_</a>
    ;

<a name="LOGICAL_AND_EXPRESSION_">BITWISE_OR_EXPRESSION_</a>
    : ^ <a href="#BITWISE_AND_EXPRESSION">BITWISE_AND_EXPRESSION</a> <a href="#BITWISE_XOR_EXPRESSION_">BITWISE_XOR_EXPRESSION_</a>
    | &epsilon;
    ; 

<a name="BITWISE_AND_EXPRESSION">BITWISE_AND_EXPRESSION</a>
    : <a href="#EQUALITY_EXPRESSION">EQUALITY_EXPRESSION</a> <a href="#BITWISE_AND_EXPRESSION_">BITWISE_AND_EXPRESSION_</a>
    ;

<a name="BITWISE_AND_EXPRESSION_">BITWISE_AND_EXPRESSION_</a>
    : & <a href="#EQUALITY_EXPRESSION">EQUALITY_EXPRESSION</a> <a href="#BITWISE_AND_EXPRESSION_">BITWISE_AND_EXPRESSION_</a>
    | &epsilon;
    ; 
    
<a name="EQUALITY_EXPRESSION">EQUALITY_EXPRESSION</a>
    : <a href="#RELATIONAL_EXPRESSION">RELATIONAL_EXPRESSION</a> <a href="#EQUALITY_EXPRESSION_">EQUALITY_EXPRESSION_</a>
    ;

<a name="EQUALITY_EXPRESSION_">EQUALITY_EXPRESSION_</a>
    : == <a href="#RELATIONAL_EXPRESSION">RELATIONAL_EXPRESSION</a> <a href="#EQUALITY_EXPRESSION_">EQUALITY_EXPRESSION_</a>
    | != <a href="#RELATIONAL_EXPRESSION">RELATIONAL_EXPRESSION</a> <a href="#EQUALITY_EXPRESSION_">EQUALITY_EXPRESSION_</a>
    | &epsilon;
    ;

<a name="RELATIONAL_EXPRESSION">RELATIONAL_EXPRESSION</a>
    : <a href="#SHIFT_EXPRESSION">SHIFT_EXPRESSION</a> <a href="#RELATIONAL_EXPRESSION_">RELATIONAL_EXPRESSION_</a>
    ;

<a name="RELATIONAL_EXPRESSION_">RELATIONAL_EXPRESSION_</a>
    : > <a href="#SHIFT_EXPRESSION">SHIFT_EXPRESSION</a> <a href="#RELATIONAL_EXPRESSION_">RELATIONAL_EXPRESSION_</a>
    | >= <a href="#SHIFT_EXPRESSION">SHIFT_EXPRESSION</a> <a href="#RELATIONAL_EXPRESSION_">RELATIONAL_EXPRESSION_</a>
    | < <a href="#SHIFT_EXPRESSION">SHIFT_EXPRESSION</a> <a href="#RELATIONAL_EXPRESSION_">RELATIONAL_EXPRESSION_</a>
    | <= <a href="#SHIFT_EXPRESSION">SHIFT_EXPRESSION</a> <a href="#RELATIONAL_EXPRESSION_">RELATIONAL_EXPRESSION_</a>
    | &epsilon;
    ; 

<a name="SHIFT_EXPRESSION">SHIFT_EXPRESSION</a>
    : <a href="#ADDITIVE_EXPRESSION">ADDITIVE_EXPRESSION</a> <a href="#SHIFT_EXPRESSION_">SHIFT_EXPRESSION_</a>
    ;

<a name="SHIFT_EXPRESSION_">SHIFT_EXPRESSION_</a>
    : >> <a href="#ADDITIVE_EXPRESSION">ADDITIVE_EXPRESSION</a> <a href="#SHIFT_EXPRESSION_">SHIFT_EXPRESSION_</a>
    | << <a href="#ADDITIVE_EXPRESSION">ADDITIVE_EXPRESSION</a> <a href="#SHIFT_EXPRESSION_">SHIFT_EXPRESSION_</a>
    | &epsilon;
    ; 

<a name="ADDITIVE_EXPRESSION">ADDITIVE_EXPRESSION</a>
    : <a href="#MULTIPLICATIVE_EXPRESSION">MULTIPLICATIVE_EXPRESSION</a> <a href="#ADDITIVE_EXPRESSION_">ADDITIVE_EXPRESSION_</a>
    ;

<a name="ADDITIVE_EXPRESSION_">ADDITIVE_EXPRESSION_</a>
    : + <a href="#MULTIPLICATIVE_EXPRESSION">MULTIPLICATIVE_EXPRESSION</a> <a href="#ADDITIVE_EXPRESSION_">ADDITIVE_EXPRESSION_</a>
    | - <a href="#MULTIPLICATIVE_EXPRESSION">MULTIPLICATIVE_EXPRESSION</a> <a href="#ADDITIVE_EXPRESSION_">ADDITIVE_EXPRESSION_</a>
    | &epsilon;
    ; 

<a name="MULTIPLICATIVE_EXPRESSION">MULTIPLICATIVE_EXPRESSION</a>
    : <a href="#UNARY_EXPRESSION">UNARY_EXPRESSION</a> <a href="#MULTIPLICATIVE_EXPRESSION_">MULTIPLICATIVE_EXPRESSION_</a>
    ;

<a name="MULTIPLICATIVE_EXPRESSION_">MULTIPLICATIVE_EXPRESSION_</a>
    : * <a href="#UNARY_EXPRESSION">UNARY_EXPRESSION</a> <a href="#MULTIPLICATIVE_EXPRESSION_">MULTIPLICATIVE_EXPRESSION_</a>
    | / <a href="#UNARY_EXPRESSION">UNARY_EXPRESSION</a> <a href="#MULTIPLICATIVE_EXPRESSION_">MULTIPLICATIVE_EXPRESSION_</a>
    | % <a href="#UNARY_EXPRESSION">UNARY_EXPRESSION</a> <a href="#MULTIPLICATIVE_EXPRESSION_">MULTIPLICATIVE_EXPRESSION_</a>
    | &epsilon;
    ; 

<a name="UNARY_EXPRESSION">UNARY_EXPRESSION</a>
    : ++ <a href="#UNARY_EXPRESSION">UNARY_EXPRESSION</a>
    | -- <a href="#UNARY_EXPRESSION">UNARY_EXPRESSION</a>
    | <a href="#UNARY_OPERATOR">UNARY_OPERATOR</a> <a href="#UNARY_EXPRESSION">UNARY_EXPRESSION</a>
    | <a href="#POSTFIX_EXPRESSION">POSTFIX_EXPRESSION</a>
    ;

<a name="UNARY_OPERATOR">UNARY_OPERATOR</a>
    : &
    | *
    | +
    | -
    | !
    ;

<a name="POSTFIX_EXPRESSION">POSTFIX_EXPRESSION</a>
    : <a href="#PRIMARY_EXPRESSION">PRIMARY_EXPRESSION</a> <a href="#POSTFIX_EXPRESSION_">POSTFIX_EXPRESSION_</a>
    ;

<a name="POSTFIX_EXPRESSION_">POSTFIX_EXPRESSION_</a>
    : [ <a href="#EXPRESSION">EXPRESSION</a> ] <a href="#POSTFIX_EXPRESSION_">POSTFIX_EXPRESSION_</a>
    | ( <a href="#POSTFIX_EXPRESSION_PARENTHESIS">POSTFIX_EXPRESSION_PARENTHESIS</a>
    | . identifier <a href="#POSTFIX_EXPRESSION_">POSTFIX_EXPRESSION_</a>
    | -> identifier <a href="#POSTFIX_EXPRESSION_">POSTFIX_EXPRESSION_</a>
    | ++ <a href="#POSTFIX_EXPRESSION_">POSTFIX_EXPRESSION_</a>
    | -- <a href="#POSTFIX_EXPRESSION_">POSTFIX_EXPRESSION_</a>
    | &epsilon;
    ;

<a name="POSTFIX_EXPRESSION_PARENTHESIS">POSTFIX_EXPRESSION_PARENTHESIS</a>
    | ) <a href="#POSTFIX_EXPRESSION_">POSTFIX_EXPRESSION_</a>
    | <a href="#ARGUMENT_LIST">ARGUMENT_LIST</a> ) <a href="#POSTFIX_EXPRESSION_">POSTFIX_EXPRESSION_</a>
    | &epsilon;
    ;

<a name="PRIMARY_EXPRESSION">PRIMARY_EXPRESSION</a>
    : identifier
    | constant
    | ( <a href="#EXPRESSION">EXPRESSION</a> )
    ;

<a name="ARGUMENT_LIST">ARGUMENT_LIST</a>
    : <a href="#ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a> <a href="#ARGUMENT_LIST_">ARGUMENT_LIST_</a>
    ;

<a name="ARGUMENT_LIST_">ARGUMENT_LIST_</a>
    : , <a href="#ASSIGNMENT_EXPRESSION">ASSIGNMENT_EXPRESSION</a> <a href="#ARGUMENT_LIST_">ARGUMENT_LIST_</a>
    | &epsilon;
    ;

</pre>
