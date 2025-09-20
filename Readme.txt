Inputs:
    Mouse1 = move camera
    Arrowkeys = move camera
    Mouse wheel = Zoom
    W = increase iterations
    S = decrease iterations
    D = next pattern
    A = previous pattern

Dataformatting:
    Name;Forward distance;Angle change;Axiom,Rules,remapping

Rules and remapping are shown in a format of a,bc where a is replaced with bc.
If there is more rules than one, seperate them with a comma.

Remapping values:
    F = draw Forward
    X = do nothing
    + = Turn Right
    - = Turn Left
    S = Split to left
    R = return to last split

Usually [ symbol is used to show a split and turn,
but in this program you have to tell them separaterly.

Example:
    binaryTree;5.0;45.0;0;0,1[0]0,1,11;0,F,1,F,[,S-,],R+

Explination:
    Name binaryTree
    Move amount: 5
    Angle amount: 45
    Axiom: 0
    Rules:
            0: 1[0]0
            1: 11
    Action map:
            0: F
            1: F
            [: S-
            ]: R+