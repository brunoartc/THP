<?php

    function soma($x, $y) {
        function echoes($b) {
            echo $b;
        }

        $a = $x + $y;
        echoes($a);
        return $a;
        echo(22);
    }

    $a = 3;
    $b = soma($a, 4);
    echoes($a);
?>