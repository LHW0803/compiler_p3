        shift_sp 1
        push_const EXIT
        push_reg fp
        push_reg sp
        pop_reg fp
        jump main
EXIT:
        exit
main:
        shift_sp 1
main_start:
        push_const Lglob+0
        push_reg sp
        fetch
        push_const 1
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 1
        add
        push_reg sp
        fetch
        push_const 10
        assign
        fetch
        shift_sp -1
str_0. string "Initial x: 1, y: 10\n"
        push_const str_0
        write_string
str_1. string "x++: "
        push_const str_1
        write_string
        push_const Lglob+0
        push_reg sp
        fetch
        push_reg sp
        fetch
        fetch
        push_const 1
        add
        assign
        fetch
        push_const 1
        sub
        write_int
str_2. string " / then: "
        push_const str_2
        write_string
        push_const Lglob+0
        fetch
        write_int
str_3. string "\n"
        push_const str_3
        write_string
str_4. string "++y: "
        push_const str_4
        write_string
        push_reg fp
        push_const 1
        add
        push_reg sp
        fetch
        push_reg sp
        fetch
        fetch
        push_const 1
        add
        assign
        fetch
        write_int
str_5. string "\n"
        push_const str_5
        write_string
main_final:
        push_reg fp
        pop_reg sp
        pop_reg fp
        pop_reg pc
main_end:
Lglob.  data 1
