        shift_sp 1
        push_const EXIT
        push_reg fp
        push_reg sp
        pop_reg fp
        jump main
EXIT:
        exit
test1:
test1_start:
        push_reg fp
        push_const -1
        add
        push_const -1
        add
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        add
        assign
        jump test1_final
test1_final:
        push_reg fp
        pop_reg sp
        pop_reg fp
        pop_reg pc
test1_end:
test2:
test2_start:
        push_reg fp
        push_const -1
        add
        push_const -1
        add
        push_reg fp
        push_const 1
        add
        fetch
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        fetch
        add
        assign
        jump test2_final
test2_final:
        push_reg fp
        pop_reg sp
        pop_reg fp
        pop_reg pc
test2_end:
test3:
test3_start:
        push_const Lglob+0
        push_reg sp
        fetch
        push_reg fp
        push_const 1
        add
        fetch
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        add
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const -1
        add
        push_const -1
        add
        push_const Lglob+0
        assign
        jump test3_final
test3_final:
        push_reg fp
        pop_reg sp
        pop_reg fp
        pop_reg pc
test3_end:
main:
        shift_sp 8
main_start:
        push_reg fp
        push_const 1
        add
        push_reg sp
        fetch
        push_const 1
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 2
        add
        push_reg sp
        fetch
        push_const 3
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 3
        add
        push_reg sp
        fetch
        shift_sp 1
        push_const label_0
        push_reg fp
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        push_reg sp
        push_const -2
        add
        pop_reg fp
        jump test1
label_0:
        assign
        fetch
        shift_sp -1
str_0. string "## [1] Basic Function Test\n"
        push_const str_0
        write_string
str_1. string "o1: "
        push_const str_1
        write_string
        push_reg fp
        push_const 3
        add
        fetch
        write_int
str_2. string " (answer is 4)\n"
        push_const str_2
        write_string
        push_reg fp
        push_const 1
        add
        push_reg sp
        fetch
        push_const 3
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 2
        add
        push_reg sp
        fetch
        push_const 4
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 4
        add
        push_reg sp
        fetch
        shift_sp 1
        push_const label_1
        push_reg fp
        push_reg fp
        push_const 1
        add
        push_reg fp
        push_const 2
        add
        push_reg sp
        push_const -2
        add
        pop_reg fp
        jump test2
label_1:
        assign
        fetch
        shift_sp -1
str_3. string "## [2] Pointer Parameter Function Test\n"
        push_const str_3
        write_string
str_4. string "o2: "
        push_const str_4
        write_string
        push_reg fp
        push_const 4
        add
        fetch
        write_int
str_5. string " (answer is 7)\n"
        push_const str_5
        write_string
        push_reg fp
        push_const 5
        add
        push_reg sp
        fetch
        shift_sp 1
        push_const label_2
        push_reg fp
        push_reg fp
        push_const 1
        add
        push_const 5
        push_reg sp
        push_const -2
        add
        pop_reg fp
        jump test3
label_2:
        fetch
        assign
        fetch
        shift_sp -1
str_6. string "## [3] Complicated Function Test\n"
        push_const str_6
        write_string
str_7. string "o3: "
        push_const str_7
        write_string
        push_reg fp
        push_const 5
        add
        fetch
        write_int
str_8. string " (answer is 8)\n"
        push_const str_8
        write_string
        push_const Lglob+1
        push_reg sp
        fetch
        push_const 6
        assign
        fetch
        shift_sp -1
        push_const Lglob+1
        push_const 1
        add
        push_reg sp
        fetch
        push_const 65
        assign
        fetch
        shift_sp -1
        push_const Lglob+1
        push_const 2
        add
        push_reg sp
        fetch
        push_const 10
        assign
        fetch
        shift_sp -1
        push_const Lglob+1
        push_const 2
        add
        push_const 1
        add
        push_reg sp
        fetch
        push_const 66
        assign
        fetch
        shift_sp -1
        push_const Lglob+1
        push_const 2
        add
        push_const 2
        add
        push_reg sp
        fetch
        shift_sp 1
        push_const label_3
        push_reg fp
        push_const Lglob+1
        fetch
        push_const Lglob+1
        push_const 2
        add
        fetch
        push_reg sp
        push_const -2
        add
        pop_reg fp
        jump test1
label_3:
        assign
        fetch
        shift_sp -1
str_9. string "## [4] Struct Test\n"
        push_const str_9
        write_string
str_10. string "st1e.x: "
        push_const str_10
        write_string
        push_const Lglob+1
        fetch
        write_int
str_11. string " | st1e.y: "
        push_const str_11
        write_string
        push_const Lglob+1
        push_const 1
        add
        fetch
        write_char
str_12. string "\n"
        push_const str_12
        write_string
str_13. string "st1e.st2e.p: "
        push_const str_13
        write_string
        push_const Lglob+1
        push_const 2
        add
        fetch
        write_int
str_14. string " | st1e.st2e.q: "
        push_const str_14
        write_string
        push_const Lglob+1
        push_const 2
        add
        push_const 1
        add
        fetch
        write_char
str_15. string "\n"
        push_const str_15
        write_string
str_16. string "st1e.st2e.st3e.a: "
        push_const str_16
        write_string
        push_const Lglob+1
        push_const 2
        add
        push_const 2
        add
        fetch
        write_int
str_17. string "\n"
        push_const str_17
        write_string
        push_reg fp
        push_const 1
        add
        push_reg sp
        fetch
        push_const 10
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 2
        add
        push_reg sp
        fetch
        push_const 20
        assign
        fetch
        shift_sp -1
str_18. string "## [5] INC/DEC Test\n"
        push_const str_18
        write_string
str_19. string "x: "
        push_const str_19
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
        push_const 1
        sub
        write_int
str_20. string " (answer is 10)"
        push_const str_20
        write_string
str_21. string " | now, x: "
        push_const str_21
        write_string
        push_reg fp
        push_const 1
        add
        fetch
        write_int
str_22. string " (answer is 11)"
        push_const str_22
        write_string
str_23. string " | and this should be, x: "
        push_const str_23
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
str_24. string " (answer is 12)\n"
        push_const str_24
        write_string
str_25. string "y: "
        push_const str_25
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
        sub
        assign
        fetch
        push_const 1
        add
        write_int
str_26. string " (answer is 20)"
        push_const str_26
        write_string
str_27. string " | now, y: "
        push_const str_27
        write_string
        push_reg fp
        push_const 2
        add
        fetch
        write_int
str_28. string " (answer is 19)"
        push_const str_28
        write_string
str_29. string " | and this should be, y: "
        push_const str_29
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
        sub
        assign
        fetch
        write_int
str_30. string " (answer is 18)\n"
        push_const str_30
        write_string
str_31. string "x++ + y++: "
        push_const str_31
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
        push_const 1
        sub
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
        add
        write_int
str_32. string " (answer is 30)"
        push_const str_32
        write_string
str_33. string " | now, x + y: "
        push_const str_33
        write_string
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        add
        write_int
str_34. string " (answer is 32)"
        push_const str_34
        write_string
str_35. string " | and this should be, ++x + ++y: "
        push_const str_35
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
        add
        write_int
str_36. string " (answer is 34)\n"
        push_const str_36
        write_string
        push_reg fp
        push_const 1
        add
        push_reg sp
        fetch
        push_const 3
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 2
        add
        push_reg sp
        fetch
        push_const 6
        assign
        fetch
        shift_sp -1
str_37. string "## [6] IF-ELSE Test\n"
        push_const str_37
        write_string
str_38. string "x: "
        push_const str_38
        write_string
        push_reg fp
        push_const 1
        add
        fetch
        write_int
str_39. string " | y: "
        push_const str_39
        write_string
        push_reg fp
        push_const 2
        add
        fetch
        write_int
str_40. string "\n"
        push_const str_40
        write_string
str_41. string "Returns true.. when |"
        push_const str_41
        write_string
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        equal
        branch_false label_4
str_42. string " x==y |"
        push_const str_42
        write_string
label_4:
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        greater_equal
        branch_false label_5
str_43. string " x>=y |"
        push_const str_43
        write_string
label_5:
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        less_equal
        branch_false label_6
str_44. string " x<=y |"
        push_const str_44
        write_string
label_6:
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        greater
        branch_false label_7
str_45. string " x>y |"
        push_const str_45
        write_string
label_7:
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        less
        branch_false label_8
str_46. string " x<y |"
        push_const str_46
        write_string
label_8:
str_47. string "\n"
        push_const str_47
        write_string
str_48. string "Returns false.. when |"
        push_const str_48
        write_string
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        equal
        not
        branch_false label_9
str_49. string " x==y |"
        push_const str_49
        write_string
label_9:
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        greater_equal
        not
        branch_false label_10
str_50. string " x>=y |"
        push_const str_50
        write_string
label_10:
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        less_equal
        not
        branch_false label_11
str_51. string " x<=y |"
        push_const str_51
        write_string
label_11:
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        greater
        not
        branch_false label_12
str_52. string " x>y |"
        push_const str_52
        write_string
label_12:
        push_reg fp
        push_const 1
        add
        fetch
        push_reg fp
        push_const 2
        add
        fetch
        less
        not
        branch_false label_13
str_53. string " x<y |"
        push_const str_53
        write_string
label_13:
str_54. string "\n"
        push_const str_54
        write_string
str_55. string "Checks for else staetements... "
        push_const str_55
        write_string
        push_reg fp
        push_const 1
        add
        fetch
        push_const 5
        greater_equal
        branch_false label_14
        push_reg fp
        push_const 2
        add
        fetch
        push_const 5
        greater
        branch_false label_15
str_56. string "(x >= 5) & (y > 5)\n"
        push_const str_56
        write_string
        jump label_16
label_15:
str_57. string "(x >= 5) & (y <= 5)\n"
        push_const str_57
        write_string
label_16:
        jump label_17
label_14:
str_58. string "(x < 5)\n"
        push_const str_58
        write_string
label_17:
str_59. string "## [7] For-Loop Test\n"
        push_const str_59
        write_string
        push_reg fp
        push_const 6
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_18:
        push_reg fp
        push_const 6
        add
        fetch
        push_const 5
        less
        branch_false label_21
        jump label_20
label_19:
        push_reg fp
        push_const 6
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
        jump label_18
label_20:
        push_reg fp
        push_const 7
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_22:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 3
        less
        branch_false label_25
        jump label_24
label_23:
        push_reg fp
        push_const 7
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
        jump label_22
label_24:
        push_const Lglob+6
        push_reg fp
        push_const 6
        add
        fetch
        push_const 3
        mul
        add
        push_reg fp
        push_const 7
        add
        fetch
        add
        push_reg sp
        fetch
        push_reg fp
        push_const 6
        add
        fetch
        push_reg fp
        push_const 7
        add
        fetch
        add
        assign
        fetch
        shift_sp -1
        jump label_23
label_25:
        jump label_19
label_21:
        push_reg fp
        push_const 6
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_26:
        push_reg fp
        push_const 6
        add
        fetch
        push_const 5
        less
        branch_false label_29
        jump label_28
label_27:
        push_reg fp
        push_const 6
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
        jump label_26
label_28:
        push_reg fp
        push_const 7
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_30:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 3
        less
        branch_false label_33
        jump label_32
label_31:
        push_reg fp
        push_const 7
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
        jump label_30
label_32:
str_60. string "st4e["
        push_const str_60
        write_string
        push_reg fp
        push_const 6
        add
        fetch
        write_int
str_61. string "]"
        push_const str_61
        write_string
str_62. string ".x["
        push_const str_62
        write_string
        push_reg fp
        push_const 7
        add
        fetch
        write_int
str_63. string "]: "
        push_const str_63
        write_string
        push_const Lglob+6
        push_reg fp
        push_const 6
        add
        fetch
        push_const 3
        mul
        add
        push_reg fp
        push_const 7
        add
        fetch
        add
        fetch
        write_int
str_64. string "\n"
        push_const str_64
        write_string
        jump label_31
label_33:
        jump label_27
label_29:
str_65. string "## [8] While-Loop Test\n"
        push_const str_65
        write_string
        push_reg fp
        push_const 6
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 7
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 8
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_34:
        push_reg fp
        push_const 6
        add
        fetch
        push_const 5
        less
        branch_false label_35
label_36:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 3
        less
        branch_false label_37
str_66. string "i: "
        push_const str_66
        write_string
        push_reg fp
        push_const 6
        add
        fetch
        write_int
str_67. string " | j: "
        push_const str_67
        write_string
        push_reg fp
        push_const 7
        add
        fetch
        write_int
str_68. string " | k: "
        push_const str_68
        write_string
        push_reg fp
        push_const 8
        add
        fetch
        write_int
str_69. string "\n"
        push_const str_69
        write_string
        push_reg fp
        push_const 7
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
        push_reg fp
        push_const 8
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
        jump label_36
label_37:
        push_reg fp
        push_const 6
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
        push_reg fp
        push_const 7
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
        jump label_34
label_35:
str_70. string "## [9-1] For-Loop Break Test\n"
        push_const str_70
        write_string
        push_reg fp
        push_const 6
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_38:
        push_reg fp
        push_const 6
        add
        fetch
        push_const 5
        less
        branch_false label_41
        jump label_40
label_39:
        push_reg fp
        push_const 6
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
        jump label_38
label_40:
        push_reg fp
        push_const 7
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_42:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 5
        less
        branch_false label_45
        jump label_44
label_43:
        push_reg fp
        push_const 7
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
        jump label_42
label_44:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 3
        equal
        branch_false label_46
        jump label_45
label_46:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 3
        greater_equal
        branch_false label_47
str_71. string "* Never Printed Line\n"
        push_const str_71
        write_string
        jump label_48
label_47:
str_72. string "st4e["
        push_const str_72
        write_string
        push_reg fp
        push_const 6
        add
        fetch
        write_int
str_73. string "]"
        push_const str_73
        write_string
str_74. string ".x["
        push_const str_74
        write_string
        push_reg fp
        push_const 7
        add
        fetch
        write_int
str_75. string "]: "
        push_const str_75
        write_string
        push_const Lglob+6
        push_reg fp
        push_const 6
        add
        fetch
        push_const 3
        mul
        add
        push_reg fp
        push_const 7
        add
        fetch
        add
        fetch
        write_int
str_76. string "\n"
        push_const str_76
        write_string
label_48:
        jump label_43
label_45:
        jump label_39
label_41:
str_77. string "## [9-2] For-Loop Continue Test\n"
        push_const str_77
        write_string
        push_reg fp
        push_const 6
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_49:
        push_reg fp
        push_const 6
        add
        fetch
        push_const 5
        less
        branch_false label_52
        jump label_51
label_50:
        push_reg fp
        push_const 6
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
        jump label_49
label_51:
        push_reg fp
        push_const 7
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_53:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 5
        less
        branch_false label_56
        jump label_55
label_54:
        push_reg fp
        push_const 7
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
        jump label_53
label_55:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 3
        equal
        branch_false label_57
        jump label_54
label_57:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 3
        greater_equal
        branch_false label_58
str_78. string "* Should be Printed if j > 3\n"
        push_const str_78
        write_string
        jump label_59
label_58:
str_79. string "st4e["
        push_const str_79
        write_string
        push_reg fp
        push_const 6
        add
        fetch
        write_int
str_80. string "]"
        push_const str_80
        write_string
str_81. string ".x["
        push_const str_81
        write_string
        push_reg fp
        push_const 7
        add
        fetch
        write_int
str_82. string "]: "
        push_const str_82
        write_string
        push_const Lglob+6
        push_reg fp
        push_const 6
        add
        fetch
        push_const 3
        mul
        add
        push_reg fp
        push_const 7
        add
        fetch
        add
        fetch
        write_int
str_83. string "\n"
        push_const str_83
        write_string
label_59:
        jump label_54
label_56:
        jump label_50
label_52:
str_84. string "## [9-3] While-Loop Break Test1\n"
        push_const str_84
        write_string
        push_reg fp
        push_const 6
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 7
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 8
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_60:
        push_reg fp
        push_const 6
        add
        fetch
        push_const 5
        less
        branch_false label_61
label_62:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 5
        less
        branch_false label_63
        push_reg fp
        push_const 7
        add
        fetch
        push_const 3
        equal
        branch_false label_64
        jump label_63
label_64:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 3
        greater_equal
        branch_false label_65
str_85. string "* Never Printed Line\n"
        push_const str_85
        write_string
        jump label_66
label_65:
str_86. string "i: "
        push_const str_86
        write_string
        push_reg fp
        push_const 6
        add
        fetch
        write_int
str_87. string " | j: "
        push_const str_87
        write_string
        push_reg fp
        push_const 7
        add
        fetch
        write_int
str_88. string " | k: "
        push_const str_88
        write_string
        push_reg fp
        push_const 8
        add
        fetch
        write_int
str_89. string "\n"
        push_const str_89
        write_string
label_66:
        push_reg fp
        push_const 7
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
        push_reg fp
        push_const 8
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
        jump label_62
label_63:
        push_reg fp
        push_const 6
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
        push_reg fp
        push_const 7
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
        jump label_60
label_61:
str_90. string "## [9-3] While-Loop Break Test2\n"
        push_const str_90
        write_string
        push_reg fp
        push_const 6
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_67:
        push_const 1
        branch_false label_68
        push_reg fp
        push_const 6
        add
        fetch
        push_const 15
        greater
        branch_false label_69
        jump label_68
label_69:
str_91. string "i: "
        push_const str_91
        write_string
        push_reg fp
        push_const 6
        add
        fetch
        write_int
str_92. string " | "
        push_const str_92
        write_string
        push_reg fp
        push_const 6
        add
        push_reg sp
        fetch
        push_reg fp
        push_const 6
        add
        fetch
        push_const 2
        add
        assign
        fetch
        shift_sp -1
        jump label_67
label_68:
str_93. string "\n"
        push_const str_93
        write_string
str_94. string "## [9-4] While-Loop Continue Test\n"
        push_const str_94
        write_string
        push_reg fp
        push_const 6
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 7
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
        push_reg fp
        push_const 8
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
label_70:
        push_reg fp
        push_const 6
        add
        fetch
        push_const 5
        less
        branch_false label_71
label_72:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 5
        less
        branch_false label_73
        push_reg fp
        push_const 7
        add
        fetch
        push_const 3
        equal
        branch_false label_74
        push_reg fp
        push_const 7
        add
        push_reg sp
        fetch
        push_const 5
        assign
        fetch
        shift_sp -1
        jump label_72
label_74:
        push_reg fp
        push_const 7
        add
        fetch
        push_const 3
        greater_equal
        branch_false label_75
str_95. string "* Never Printed Line\n"
        push_const str_95
        write_string
        jump label_76
label_75:
str_96. string "i: "
        push_const str_96
        write_string
        push_reg fp
        push_const 6
        add
        fetch
        write_int
str_97. string " | j: "
        push_const str_97
        write_string
        push_reg fp
        push_const 7
        add
        fetch
        write_int
str_98. string " | k: "
        push_const str_98
        write_string
        push_reg fp
        push_const 8
        add
        fetch
        write_int
str_99. string "\n"
        push_const str_99
        write_string
label_76:
        push_reg fp
        push_const 7
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
        push_reg fp
        push_const 8
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
        jump label_72
label_73:
        push_reg fp
        push_const 6
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
        push_reg fp
        push_const 7
        add
        push_reg sp
        fetch
        push_const 0
        assign
        fetch
        shift_sp -1
        jump label_70
label_71:
main_final:
        push_reg fp
        pop_reg sp
        pop_reg fp
        pop_reg pc
main_end:
Lglob.  data 21
