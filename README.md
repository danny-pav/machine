# machine

How to use this:
The classes are used to implement a state machine.  The machine works by consuming input, one token at a time, and traversing from state to state based on the token and performing actions based on the transition and the state.

Machine - the state machine

State - an individual state

Link - a directed transition from one state to another state (or the same state).

Input - the data to be read or event source of the state machine 

Output - the place for the data to be written from the state machine

Token - the chunk of data from each read or an event

Advance - the function to extract a token from an input

Filter - a function to determine if a token meets a certain criteria

Iterator - a cursor over a process

1. Determine the input.  If this is being used to for parsing, the input should hold the state of where the read point is.  If this is not needed, use Nothing.
2. Determine the token.  This is the chunk of data that is read or an event.
3. Determine the output.  This can be where output data gets written.  If this is not needed, use Nothing.
4. Declare a type using Machine<Input, Token, Output>.
5. Determine the states in the diagram.  Create them.  Add an action to the state if an action should be taken when arriving at that state from another.  (This means that the action will not be taken at start for the start state,.)
6. Determine the links in the diagram.  Create them. These link from and to the states declared.  Each link should have a filter.  Using NoFilter is the equivalent to accepting all.  Filters are used to determine the next state from each state.  A token will be filtered in each link in the order in which they are declared and the first link that passes will be followed.  If no links are found to pass, an error will occur.  Also determine any action to be taken when following that link.
7. Determine the function that will be used to get the token from the input.  This function should have the signature: void function(TInput&, TToken&);
8. Create the machine using the type, a start state, the stop state and the token reader function.
9. Use the machine to process input of type TInput and output of TOutput.  Function will return true if successful, false if error occurs.
10. Use the machine to process one step at a time by creating an Iterator using the machine, input and output.  Then repeatedly calling processStep.

Notes: the classes and templates have no allocation and the processing is done in a const manner.  This allows the machine, states and links to be static objects.


