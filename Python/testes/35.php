<?php
{
    function soma($x, $y) {
        function echoes($b) {
            echo $b;
            }
        $a = $x + $y;
        echoes($a);
        return $a;
        }
    $a = 3;
    $b = soma($a, 4);
    echo $b;
    echoes($a);
    $c = soma($b, $a); 
    
}
?>