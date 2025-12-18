        shift_sp 1
        push_const EXIT
        push_reg fp
        push_reg sp
        pop_reg fp
        jump main
EXIT:
        exit
main:
        shift_sp 3
main_start:
        push_reg fp
        push_const 1
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 2
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 3
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_0:
        push_reg fp
        push_const 1
        add
        fetch
        push_const 10
        less_equal
        branch_false label_1
label_2:
        push_reg fp
        push_const 2
        add
        fetch
        push_const 5
        less
        branch_false label_3
        push_reg fp
        push_const 2
        add
        fetch
        push_const 3
        equal
        branch_false label_4
        push_reg fp
        push_const 2
        add
        push_reg sp
        fetch
        push_const 4
        assign
        fetch
        shift_sp -1
        jump label_2
label_4:
        push_reg fp
        push_const 2
        add
        fetch
        write_int
str_0. string "\n"
        push_const str_0
        write_string
        push_reg fp
        push_const 2
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
        push_const 1
        sub
        shift_sp -1
        jump label_2
label_3:
        push_reg fp
        push_const 3
        add
        push_reg sp
        fetch
        push_reg fp
        push_const 3
        add
        fetch
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
        push_const 1
        sub
        add
        assign
        fetch
        shift_sp -1
        jump label_0
label_1:
        push_reg fp
        push_const 3
        add
        fetch
        write_int
str_1. string "\n"
        push_const str_1
        write_string
main_final:
        push_reg fp
        pop_reg sp
        pop_reg fp
        pop_reg pc
main_end:
Lglob.  data 0
