-------------------------------- SUMMARY --------------------------------

I] Presentation

II] IAssembly (.iasm)
    1) Basic structure
    2) Instruction set

III] IA executable codes (.iax)
    1) Introduction
    2) IAPC8's architecture
    3) IAPC8's PC-Variables
    4) Code explanation












-------------------------------- I] Presentation --------------------------------

    1) Introduction

    Thank you for downloading IAPC8_Compiler[0.1.5],

    This program is a compiler for IAPC8[0.1.5] scripts.
    IAPC8 is an 8 bit computer made in Minecraft using basic redstone components
    such as redstone dust, repeaters (wires) and torches (inverters).

    This computer can execute programs coded in IAssembly language, a language
    made specially for it (inspired on assembly but really easy to learn).
    All you have to know about IAssembly is written after this presentation.

    What you have to do is just to compile your IAssembly code with this program
    and use the compiled one in Minecraft !

    Moreover, this compiler allows you to compile your code in executable-like code.
    This format can be used in your own machines on whatever support you want !
    This is containing the most basic instructions a computer can understand.
    All details about this format are given after this presentation and the "IA executable codes" section.

    Compilation options :
        YOUR_CODE.iasm (Your code in IAssembly)
         |                              |
         |                              |
         |                              | Executable compilation
         |                              | (command "exe-compile")
         |                              |
         |                              V
         | Full compilation           YOUR_CODE.iax (Your code compiled as an executable program)
         | (command "full-compile")     |
         |                              |
         |                              | Minecraft compilation
         |                              | (command "mc-compile")
         |                              |
         |                              |
         V                              V
        YOUR_CODE.iamc (Your code ready to be used on Minecraft !)

    To run Compiler program, you just need to run the program named "run" in the current folder.

    All the program files are stored in their corresponding folder :
        "YOUR_CODE.iasm" MUST BE in folder 1_IASM/
        "YOUR_CODE.iax"  will be in folder 2_IAX/
        "YOUR_CODE.iamc" will be in folder 3_IAMC/

    The main objective of IAPC8 is to be able to execute any kind of action only using
    basic electronic stuff (basically wires and inverters).
    Minecraft is not made nor optimizated for this.
    So the execution of your code can be a bit slow depending on your computer components.

    What you can expect from IAPC8 is only to see basic drawings on an 16x8 pixels screen
    and display a value on 2 hexadecimal digits in game.


    I hope you will enjoy using this stuff to create your own programs and see what's
    happenning in the brain of a computer !

    For more information about IAPC8, please check out the presentation video :
        https://www.youtube.com/playlist?list=PLQEqOZghDI7wZngVOl4vHan5x4nuBEZ1S

    Don't forget to subscribe for more stuff like this !

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                                         By I.A.












-------------------------------- II] IAssembly (.iasm) --------------------------------

1) Basic structure

    An IAssembly code must respect the structure rules below :

        - The code has to be written as normal text in a basic text file,
          line by line, one instruction per line.

        - Each instruction has to start with 3 letters (minimal or capital)
          These letters are called the COMMAND (or cmd).
    
        - The COMMAND must be followed by 2 ARGUMENTS
          (even if only one is useful for the current COMMAND).

        - Each ARGUMENT is a value between 0 and 255 that can be
          written in 3 different bases : decimal, hexadecimal or binary.
          > Hexadecimal : character 'x' followed by 2 digits
                          (for letters, minimal or capital are accepted).
          > Decimal     : character 'd' followed by 3 digit
                          (be careful, don't put more than 255).
          > Binary      : no character before, only 8 digits
                          (default base is binary).

        - ARGUMENTS can have the prefix 'a' (minimal only) before their value.
          It means that the value of ARGUMENT will not be used as a regular number
          but used as an address in data storage.
          So the value of the ARGUMENT is the value currently stored in the
          focused data memory box.

        - ARGUMENTS can have the value 'r' (minimal only) instead of a value.
          It means that the value of ARGUMENT will be the current result of the ALU.
          It cannot be used with the prefix 'a' (it doesn't make sens).

        - Only text characters and underscores will be seen as "code"
          by the compiler (alphabet + numbers + '_').
          This allows you to use spaces and special characters as you want
          anywhere in your code.

        - PAY ATTENTION TO THE CHARACTER '#', this is a special character used
          to start a comment zone.
          After this character, your code will not be seen by the compiler until
          the next character '#' found OR until the end of the current line.

        - By the way, if the compiler understands a line of your code, it will
          ignore the rest of the line and immediatelly go to the next one.

        Example :
            ASG  x00  d001
            ASG ax00  00000110
            ASG  d003 d112
            ADD  x04  d255 # this is a comment at end of line
            ASG            # this is a comment in the middle of the line # x00 x01
            PXL  xff v
            PXL  xFF v #this is exactly the same line as the previous one
            ASG x00 r
            ASG x01 x01 this is text that will not be read by the compiler too



2) Instruction set

    Instruction set is segmented in several sections :
    DAT (Data), ALU (Arithmetic and Logic Unit), 8OP (Operations on 8 bits),
    CMP (Comparations), CUU (Computer to User Unit),
    PCO (Program Counter Operations), UOP (User Output)

    DAT
        ASG arg1 arg2 : store the value of arg2 in the memory box addressed arg1

    ALU
        8OP
            INV arg1      : set value of r at NOT arg1      (bitwise "not")
            AND arg1 arg2 : set value of r at arg1 AND arg2 (bitwise "and")
            LSH arg1 arg2 : set value of r at arg1 * 2^arg2 (bitwise "left  shift" arg2 times)
            RSH arg1 arg2 : set value of r at arg1 / 2^arg2 (bitwise "right shift" arg2 times)
            AD1 arg1      : set value of r at arg1 + 1
            ADD arg1 arg2 : set value of r at arg1 + arg2
            MUL arg1 arg2 : set value of r at arg1 * arg2
            DIV arg1 arg2 : set value of r at arg1 / arg2 (not available yet)
        CMP
            INFORMATION > if the comparation is true, r is set to 255, else to 0
            EZ_ arg1      : comparation true if arg1 equals 0
            EN_ arg1 arg2 : comparation true if arg1 equals arg2
            GZ_ arg1      : comparation true if arg1 is greater than 0 (strict)
            LZ_ arg1      : comparation true if arg1 is less than 0    (strict)

    CUU
        PCO
            IF_ arg1 arg2 : if arg1 equals 0, skip the arg2 next lines
            JMP arg1      : execution goes to curent line + arg1
        UOP
            PXL arg1      : toggle a pixel on screen (UOP_IN) following this syntax :
                            YYYXXXXT
                            Y coordinate on 3 bits
                            X coordinate on 4 bits
                            T if ON, toggle the pixel (Else, do nothing).

    That's all about IAssembly !
    You can find step by step tutorials on IAssembly on my Youtube Channel

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                                         By I.A.












-------------------------------- III] IA executable codes (.iax) --------------------------------

This section is about IA executable codes (finishing with .iax).

1) Introduction

It is important for you to know that this format is NOT MADE FOR MINECRAFT only.
This format is made for ANY KIND OF COMPUTER on ANY KIND OF SUPPORT.
The only rule to make it work is, for your computer to have the same ARCHITECTURE type.

I highly encourage you to make your own simulators to make these codes work on it !
I tried to make it the most simple for a computer to understand it.



2) IAPC8's architecture

IAPC8 is globally built in 3 big parts

    DAT : Data, this is only a storage space
          (Maximum 256 bytes of storage)

    ALU : Arithmetic and Logic Unit, this is where all kind of calculations
          are made.
          (Contains 8 bits Operations "8OP" and Comparations "CMP").

    CUU : Computer to User Unit, this is all the stuff that allows the user
          to control the whole computer.
          (Contains Program Counter Operations "PCO" and User Output modules "UOP")

    +-------------+
    |    IAPC8    |
    +-------------+-------------+--------------+
    |     DAT     |     ALU     |      CUU     |
    |             |     / \     |      / \     |
    |             |  CMP   8OP  |   PCO   UOP  |
    +-------------+-------------+--------------+
         ^   v         ^   v        ^   v    ^      +------+
         |___|_________|___|________|___|    |______| User |
                                                    +------+



3) IAPC8's PC-Variables

        The computer works ONLY by setting values to PC-Variables.
        This is done from the CUU interface.
        This means that if your computer is correctly made, you only need this
        interface to control all of it.

        The CUU interface must allows you to manipulate your data as you want,
        from the DAT to the ALU and in the opposite sens,
        from the ALU to the CUU and in the opposite sens,
        from the CUU to the DAT and in the opposite sens.

        PC-Variables are the only thing that must stay from your code, if there is
        something else, then your executable code is CORRUPTED and will probably DON'T WORK.
        The following list shows you all the IAPC8 PC-Variables and their links :

+==============+
| PC-Variables |
+==============+====+====================+========+============================+==============================================+
|     Name     | ID |    Signification   |  Type  |       Connections          |                   Utility                    |
+==============+====+====================+========+============================+==============================================+
! CUU_IN       ! 00 ! "CUU INPUT"        ! 8 bits !     DAT_OUT -> CUU_IN      ! Information comming from DAT_OUT and         !
!              !    !                    !        !     ALU_OUT                ! ALU_OUT to user.                             !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_OUT      ! 01 ! "CUU OUTPUT"       ! 8 bits !     CUU_OUT -> ALU_IN/OP   ! Information comming from user to ALU_IN,     !
!              !    !                    !        !                DAT_IN      ! ALU_OP and DAT_IN.                           !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_DAT_A    ! 02 ! "CUU DAT ADDRESS"  ! 8 bits !   CUU_DAT_A -> DAT_ADR     ! Address port used from user to data (DAT).   !
!              !    !                    !        !                            !                                              !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_DAT_S    ! 03 ! "DAT SET"          ! 1 bit  !   CUU_DAT_S -> DAT_SET     ! if ON, value in DAT_IN will be stored at     !
!              !    !                    !        !                            ! address DAT_ADR.                             !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_DAT_G    ! 04 ! "DAT GET"          ! 1 bit  !    CUU_DAT_G -> DAT_GET    ! if ON, DAT_OUT will now have the value       !
!              !    !                    !        !                            ! stored at address DAT_ADR.                   !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_DAT_IG   ! 05 ! "DAT IN GATE"      ! 1 bit  ! CUU_DAT_IG -> DAT_IN_GATE  ! if ON, CUU_OUT and ALU_OUT will now share    !
!              !    !                    !        !                            ! their value to DAT_IN.                       !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_DAT_OG   ! 06 ! "DAT OUT GATE"     ! 1 bit  ! CUU_DAT_OG -> DAT_OUT_GATE ! if ON, DAT_OUT will share its value with     !
!              !    !                    !        !                            ! ALU_IN and CUU_IN.                           !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_ALU_SS   ! 07 ! "ALU SECOND SET"   ! 1 bit  ! CUU_ALU_SS -> ALU_SS       ! if ON, second operator of ALU will be set to !
!              !    !                    !        !                            ! ALU_IN value. Else, this is for the first.   !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_ALU_OS   ! 08 ! "ALU OUT SET"      ! 1 bit  ! CUU_ALU_OS -> ALU_OS       ! if ON, result of ALU will be written in      !
!              !    !                    !        !                            ! ALU_OUT.                                     !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_ALU_PS   ! 09 ! "ALU OPERATOR SET" ! 1 bit  ! CUU_ALU_PS -> ALU_OPS      ! if ON, ALU_OP will be set to CUU_OUT.        !
!              !    !                    !        !                            !                                              !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_ALU_OG   ! 10 ! "ALU OUT GATE"     ! 1 bit  ! CUU_ALU_OG -> ALU_OUT_GATE ! if ON, ALU_OUT will share its value with     !
!              !    !                    !        !                            ! CUU_IN and DAT_IN.                           !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_ALU_OF   ! 11 !"ALU OVERFLOW"      ! 1 bit  ! CUU_ALU_OF <- ALU_OF       ! if ON, result of ALU detects an overflow.    !
!              !    !                    !        !                            !                                              !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_PCO_OF   ! 12 ! "PCO OVERFLOW"     ! 1 bit  ! CUU_PCO_OF <- PCO_OF       ! if ON, PCO detects an overflow.              !
!              !    !                    !        !                            !                                              !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_PCO_S    ! 13 ! "PCO SET"          ! 1 bit  !  CUU_PCO_S -> PCO_SET      ! if ON, result of PCO operation will be       !
!              !    !                    !        !                            ! written in Program Counter.                  !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_PCO_B    ! 14 ! "PCO BRIDGE"       ! 1 bit  !  CUU_PCO_B -> PCO_B        ! if ON, PCO_SET will be configurable to ON    !
!              !    !                    !        !                            ! only if CUU_IN is not 00000000.              !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_PCO_A    ! 15 ! "PCO ADD"          ! 1 bit  !  CUU_PCO_A -> PCO_ADD      ! if ON, the PCO switch in ADD operation       !
!              !    !                    !        !                            ! mode (if OFF, PCO operation is AD1).         !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_PCO_I    ! 16 ! "PCO INPUT"        ! 8 bits ! CUU_PCO_IN -> PCO_IN       ! Informations comming from user to Program    !
!              !    !                    !        !                            ! Counter.                                     !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_UOP_S    ! 17 ! "UOP SET"          ! 1 bit  !  CUU_UOP_S -> UOP_SET      ! if ON, UOP_IN will be set to CUU_IN.         !
!              !    !                    !        !                            !                                              !
+--------------+----+--------------------+--------+----------------------------+----------------------------------------------+
! CUU_UOP_I    ! 18 ! "UOP INPUT"        ! 8 bits !  CUU_UOP_I -> UOP_IN       ! Information comming from user to Display.    !
!              !    !                    !        !                            !                                              !
+==============+====+====================+========+============================+==============================================+

        You may notice that all these variables starts with "CUU", this is because
        these variables are accessed by the CUU interface.

        /!\ BE CAREFUL /!\ : When using these PC-Variables, mind that you might have to reset them all
        after each operation if you don't want to have bad sursprises !
        The only PC-Variable that don't need to be reset is CUU_PCO_S (it is automatically reset by IAPC8).



4) Code explanation

    Each PC-Variable is refered by an ID number written on 2 digits (in decimal).
    Each line of an IA executable file is a value affectation on 8 bits EVEN IF the current PC-Variable is
    only written on 1 bit.

    Here is the code line structure : XXYYYYYYYY

    XX : PC-Variable ID
    YYYYYYYY : Value on 8 bits (if PC-Variable is on 1 bit only : 0000000Y)

    +------------+
    |   Example  |
    +------------+-----------------------+----------------------------------------------+
    | Executable |     Signification     |                 Explanation                  |
    +------------+-----------------------+----------------------------------------------+
    | 0110010000 | CUU_OUT    = 10010000 | Push value 144 in ALU_IN/ALU_OP/DAT_IN.      |
    | 0900000001 | CUU_ALU_PS = 1        | Set ALU operator to CUU_OUT.                 | (operation number 144 is ADD)
    | 0900000000 | CUU_ALU_PS = 0        | Set done.                                    |
    | 0100000010 | CUU_OUT    = 00000010 | Push value 2 in ALU_IN/ALU_OP/DAT_IN.        |
    | 0700000001 | CUU_ALU_SS = 1        | Set second ALU operand to ALU_IN.            |
    | 0111111111 | CUU_OUT    = 11111111 | Push value -1 in ALU_IN/ALU_OP/DAT_IN.       | (calculation is made instantly)
    | 0800000001 | CUU_ALU_OS = 1        | Push result of the ALU into ALU_OUT.         |
    | 0800000000 | CUU_ALU_OS = 0        | Set done (for ALU_OUT).                      |
    | 1000000001 | CUU_ALU_OG = 1        | Push ALU_OUT into CUU_IN.                    |
    | 1700000001 | CUU_UOP_S  = 1        | Push CUU_IN into UOP_IN.                     | (2+-1=1 -> will be written on UOP_IN)
    | 0700000000 | CUU_ALU_SS = 0        | Set first ALU operand to ALU_IN. (reset)     |
    | 1000000000 | CUU_ALU_OG = 0        | Close ALU_OUT gate.                          |
    +------------+-----------------------+----------------------------------------------+

    This example will make the calculation 2+-1 => 1 and push it into the screen.
    (value 00000001 into UOP_IN)

    +-------------+
    | ALU OPCODES |
    +-------------+------+-----------+
    |    Value    | Type | Operation |
    +-------------+------+-----------+
    |  0000 0000  |  CMP |    EZ_    |
    |  1000 0000  |  CMP |    EN_    |
    |  0100 0000  |  CMP |    GZ_    |
    |  1100 0000  |  CMP |    LZ_    |
    |  0010 0000  |  CMP |     -     | (operations not defined)
    |  1010 0000  |  CMP |     -     |
    |  0110 0000  |  CMP |     -     |
    |  1110 0000  |  CMP |     -     |
    |  0001 0000  |  8OP |    ADD    |
    |  1001 0000  |  8OP |    AD1    |
    |  0101 0000  |  8OP |    MUL    |
    |  1101 0000  |  8OP |    INV    |
    |  0011 0000  |  8OP |    AND    |
    |  1011 0000  |  8OP |    LSH    |
    |  0111 0000  |  8OP |    RSH    |
    |  1111 0000  |  8OP |    DIV    |
    +-------------+------+-----------+


    Now you know how to run IA Executable files !
    I hope you will enjoy using these technologies in your own projects, have fun !

    Contact     : i.a.sebsil83@gmail.com
    Youtube     : https://www.youtube.com/user/IAsebsil83
    GitHub repo : https://github.com/iasebsil83

    Let's Code !                                                         By I.A.
