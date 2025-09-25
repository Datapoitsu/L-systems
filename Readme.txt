Inputs:
    Mouse1 = move camera
    Arrowkeys = move camera
    Mouse wheel = Zoom
    W = increase iterations
    S = decrease iterations
    D = next pattern
    A = previous pattern

Warning! The program doesn't contain any limits. Increasing iterations can quickly lead to lag or a crash.

Dataformatting:
    Name;Forward distance;Angle change;Axiom,Rules,remapping

Rules and remapping are shown in a format of a:bc where a is replaced with bc.
If there is more rules than one, seperate them with a comma.

Remapping values:
    F = Draw forward
    f = Forward without drawing
    H = draw halfstep forward
    h = halfstep forward without drawing
    X = do nothing
    + = Turn Right
    - = Turn Left
    | = Turn 180 deg
    [ = Push new state to stack -> "Split"
    ] = Pop last state from the stack -> Come back to the splitted position.

Sometimes [ symbol is used to show a split and turn, in this program you have to tell them separaterly.

Example:
    binaryTree;5.0;45.0;0;0:1[0]0,1:11;0:F,1:F,[:[-,]:]+

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
            [: [-
            ]: ]+