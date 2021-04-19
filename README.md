# Parser-and-Tree-Builder

Parser and Tree Builder

Invocation:

frontEnd [file]
with file as token scanner with the same implicit extension.

Assume the project grammar is LL(1) . You need to compute First() sets as needed (for any nonterminal that has multiple productions). If a token doesn't match any of the sets in the function for the nonterminal, issue an appropriate error; however, if there is the empty production then instead of the error you use the empty production - return from the function.

Any error will exit the entire program after displaying the appropriate message.

Implement the parser in a separate file (parser.c and parser.h for example if in C) including the initial auxiliary parser() function and all nonterminal functions. Call the parser function from main after all the checks on the main arguments and anything needed to initiate your scanner.

The parser function generates an error or returns the parse tree to main. The main will then call a testing function to print the tree. The printing should be in the same file as the parser or in a separate file. The printing function will use preorder traversal with indentations, as before, for testing purposes

Each node will print one line or possibly nothing (if the leave has nothing in it).
The line will start with indentations of 2 spaces per level, then display the node's label and any tokes stored in the node
Continue over children left to right
The printing function call must be later disabled and used only for debugging.

This program will be tested assuming white spaces separate all tokens and tested on file input only


**BNF GRAMMAR
(Please ensure this uses only tokens detected in existing token scanner. Note that some legal tokens will pass the scanner but will always lead to an error here as they are not part of the BNF.)

<program>  ->     <vars> main <block>
<block>       ->      begin <vars> <stats> end
<vars>         ->      empty | data Identifier :=  Integer  ;  <vars>
<expr>        ->      <N> - <expr>  | <N>
<N>             ->       <A> / <N> | <A> * <N> | <A>
<A>             ->        <M> + <A> | <M>
<M>              ->     * <M> |  <R>
<R>              ->      ( <expr> ) | Identifier | Integer
<stats>         ->      <stat>  <mStat>
<mStat>       ->      empty |  <stat>  <mStat>
<stat>           ->      <in> ;  | <out> ;  | <block> | <if> ;  | <loop> ;  | <assign> ; | <goto> ; | <label> ;
<in>              ->      getter  Identifier
<out>            ->      outter <expr>
<if>               ->      if [ <expr> <RO> <expr> ] then <stat>
<loop>          ->      loop  [ <expr> <RO> <expr> ]  <stat>
<assign>       ->      assign Identifier  := <expr>
<RO>            ->      < | =>  | =< |  ==  |   [ == ]  (three tokens)  | %
<label>          ->    void Identifier
<goto>           ->    proc Identifier


**TEST PROGRAMS

Testing - Good Programs
To test you want to create files using the grammar, starting with the simplest programs one different statement at a time, building up sequences of statements, nested statements and expressions, etc. You may skip comments but then test a comment in some files. Make sure to have sequences of statements, nested statements, nested ifs and loops, variables in various blocks, expressions and to test all operators.



Below are 6 example files. These programs should NOT generate scanner or parser errors.



The first tests you do should be parser only, no tree generation. Then the final test should do the same, except generate a printed tree and verify that the tree is correct. Each tree needs to have all semantic tokens and the proper structure corresponding to the program.

First group: shortest programs with one statement, replace print with one other statement at a time, including blocks as shown, then adding single variable declarations.


$$ p2g1, remember to try variants of this with each different statement $$
main
begin
getter x ;
end

$$ p2g2 $$
data x := 5 ;
main
begin
getter x ;
end

$$ p2g3 $$
data x := 5 ;
main
begin
data y := 67 ;
outter y ;
end

$$ p2g4 $$
data z := 321 ;
main
begin
data y := 5 ;
loop [ y => z ]
    assign x := y ; ;
outter x ;
end

$$ p2g5 $$
data _z := 321 ;
data y := 567 ;
main
begin
data y := 5 ;
loop [ y => z ]
  begin
  data z := 3 ;
     begin
     proc x ;
     end
  end ;
outter x ;
end

$$ p2g6, lastly, you want to very thoroughly test expressions with variations of below$$
data a := 5 ;
data b := 6 ;
data c := 7 ;
main
begin
    outter ( 2 * 3 / 5 + 7 - * 11 + ( 13 * * 17 ) ) ;
end


