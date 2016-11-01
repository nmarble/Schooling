data

array:
 .word 0x01234567
 .word 0x12345678
 .word 0x00012345
 .word 0x00123456
 .word 0x00000012
 .word 0x00000123
 .word 0x456789AB
 .word 0x23456789
 .word 0x3456789A
 .word 0x00000001
 .word 0x00001234
 
nElements:
 .word 0xb
 
a0NotOK:
 .asciiz "Register $a0 values were not preserved\r\n"
 
a1NotOK:
.asciiz "Register $a1 values were not preserved\r\n"

OutOfOrder:
.asciiz "Array elements out of order\r\n"

Errors:
.asciiz " Errors\r\n"

GoodJob:
.asciiz "Nicely done. Thanks!\r\n"
.text
la $a0, array#initialize $a0 and $a1 for call
la $t0, nElements
lw $a1, 0($t0)
addi $s0, $a0, 0 #save for comparison later
addi $s1, $a1, 0
jal BubbleSort #sort the array
addi $t5, $0, 0 #counts errors
beq $a0, $s0, a0OK #check results
addi $t5, $t5, 1 #increment error counter
la $a0, a0NotOK
li $v0, 4
syscall

a0OK:
beq $a1, $s1, a1OK
addi $t5, $t5, 1 #increment error counter
la $a0, a1NotOK
li $v0, 4
syscall

a1OK:
la $t0, nElements#initialize counter
lw $t1, 0($t0)
addi $t1, $t1, -1
la $t0, array

CheckLoop:
lw $t2, 0($t0) #load adjacent elements
lw $t3, 4($t0)
slt $t4, $t3, $t2 #compare them
beq $t4, $0, LineOK
addi $t5, $t5, 1 #increment error counter
la $a0, OutOfOrder
li $v0, 4
syscall

LineOK:
addi $t0, $t0, 4
addi $t1, $t1, -1
bne $t1, $0, CheckLoop
beq $t5, $0, AllOK
addi $a0, $t5, 0
addi $v0, $0, 1
syscall
la $a0, Errors
li $v0, 4
syscall
j AllDone

AllOK:
la $a0, GoodJob
li $v0, 4
syscall

AllDone:
addi $v0, $0, 10 #end of program
syscall

BubbleSort:
la $t0, ($a0) #Address of beggining of array
la $t1, ($a1) #Number of objects to search
li $t6,0 #Stores if the array is out of order

loop:
addi $t5, $t5, 1 #increment current index
bge $t5, $t1, rescan #checks if at end of array
lw $t2,($t0) #load current
lw $t3,4($t0) #loads next
bge $t3,$t2,next #branch if they are in the right order
sw $t3,($t0) #Put next into current
sw $t2,4($t0) #Put current into next
li $t6,1 #Confirms array is still out of order

next:
addi $t0,$t0,4 #Increments to next value
j loop

rescan: 
beqz $t6,complete #If it wasnt confimed it was out of order end.
li $t5, 0 #set index back to 0
subi $t1, $t1, 1 #decrement search size
la $t0, ($a0) #Resets the start of the array
li $t6,0 #set that the array is in order
j loop

complete:
jr $ra #All Done!
