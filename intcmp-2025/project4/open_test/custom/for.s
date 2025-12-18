        shift_sp 1
        push_const EXIT
        push_reg fp
        push_reg sp
        pop_reg fp
        jump main
EXIT:
        exit
main:
        shift_sp 21
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
label_0:
        push_reg fp
        push_const 1
        add
        fetch
        push_const 10
        less_equal
        branch_false label_3
        jump label_2
label_1:
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
        shift_sp -1
        jump label_0
label_2:
        push_const Lglob+0
        push_reg fp
        push_const 1
        add
        fetch
        add
        push_reg sp
        fetch
        push_reg fp
        push_const 1
        add
        fetch
        assign
        fetch
        shift_sp -1
        jump label_1
label_3:
        push_reg fp
        push_const 1
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_4:
        push_reg fp
        push_const 1
        add
        fetch
        push_const 10
        less_equal
        branch_false label_7
        jump label_6
label_5:
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
        shift_sp -1
        jump label_4
label_6:
        push_reg fp
        push_const 2
        add
        push_reg fp
        push_const 1
        add
        fetch
        add
        push_reg sp
        fetch
        push_const 100
        push_const 10
        push_reg fp
        push_const 1
        add
        fetch
        mul
        add
        assign
        fetch
        shift_sp -1
        jump label_5
label_7:
        push_reg fp
        push_const 1
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_8:
        push_reg fp
        push_const 1
        add
        fetch
        push_const 10
        less
        branch_false label_11
        jump label_10
label_9:
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
        shift_sp -1
        jump label_8
label_10:
        push_reg fp
        push_const 12
        add
        push_reg fp
        push_const 1
        add
        fetch
        add
        push_reg sp
        fetch
        push_const Lglob+0
        push_reg fp
        push_const 1
        add
        fetch
        add
        fetch
        push_reg fp
        push_const 2
        add
        push_reg fp
        push_const 1
        add
        fetch
        add
        fetch
        add
        assign
        fetch
        shift_sp -1
        jump label_9
label_11:
        push_reg fp
        push_const 1
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_12:
        push_reg fp
        push_const 1
        add
        fetch
        push_const 10
        less
        branch_false label_15
        jump label_14
label_13:
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
        shift_sp -1
        jump label_12
label_14:
str_0. string "x: "
        push_const str_0
        write_string
        push_const Lglob+0
        push_reg fp
        push_const 1
        add
        fetch
        add
        fetch
        write_int
str_1. string " | y: "
        push_const str_1
        write_string
        push_reg fp
        push_const 2
        add
        push_reg fp
        push_const 1
        add
        fetch
        add
        fetch
        write_int
str_2. string " | z: "
        push_const str_2
        write_string
        push_reg fp
        push_const 12
        add
        push_reg fp
        push_const 1
        add
        fetch
        add
        fetch
        write_int
str_3. string "\n"
        push_const str_3
        write_string
        jump label_13
label_15:
main_final:
        push_reg fp
        pop_reg sp
        pop_reg fp
        pop_reg pc
main_end:
Lglob.  data 10
