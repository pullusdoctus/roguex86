section .text
global _hasAnyConnection         ; checks if a room has any connections
global _getOppositeDirection     ; gets the opposite direction
global _shuffleDirections        ; shuffles array of directions

; bool _hasAnyConnection(void** adjacentRooms, int numDirections)
; Checks if a room has any connections
; Parameters:
;   adjacentRooms - Array of pointers to adjacent rooms (rdi)
;   numDirections - Number of directions to check (esi)
; Returns:
;   1 if any connections exist, 0 if no connections
_hasAnyConnection:
    xor eax, eax        ; Initialize result to 0 (false)
    xor ecx, ecx        ; Initialize counter

.check_next:
    cmp ecx, esi        ; Check if we've checked all directions
    jge .done           ; If so, done
    cmp qword [rdi + rcx*8], 0    ; Check if this direction has a connection
    jne .has_connection ; If non-null, there's a connection
    inc ecx             ; Increment counter
    jmp .check_next     ; Check next direction
.has_connection:
    mov eax, 1          ; Set result to 1 (true)
.done:
    ret                 ; Return result in eax

; int _getOppositeDirection(int direction)
; Gets the opposite direction (NORTH<->SOUTH, EAST<->WEST)
; Parameters:
;   direction - Direction enum value (edi)
; Returns:
;   Opposite direction enum value
_getOppositeDirection:
    ; Direction constants (from the game):
    ; NORTH = 0, SOUTH = 1, WEST = 2, EAST = 3
    mov eax, edi        ; Copy direction to eax
    ; For NORTH (0) -> SOUTH (1)
    ; For SOUTH (1) -> NORTH (0)
    ; For WEST (2) -> EAST (3)
    ; For EAST (3) -> WEST (2)
    cmp eax, 0          ; NORTH?
    je .north_to_south
    cmp eax, 1          ; SOUTH?
    je .south_to_north
    cmp eax, 2          ; WEST?
    je .west_to_east
    cmp eax, 3          ; EAST?
    je .east_to_west
    ; Invalid direction, return -1
    mov eax, -1
    ret
.north_to_south:
    mov eax, 1          ; SOUTH
    ret
.south_to_north:
    xor eax, eax        ; NORTH
    ret
.west_to_east:
    mov eax, 3          ; EAST
    ret
.east_to_west:
    mov eax, 2          ; WEST
    ret
; void _shuffleDirections(int* directions, int count, unsigned int seed)
; Shuffles an array of directions using Fisher-Yates algorithm
; Parameters:
;   directions - Array of direction values (rdi)
;   count - Number of elements (esi)
;   seed - Random seed (edx)
; Returns:
;   None (shuffles the array in-place)
_shuffleDirections:
    ; Using a simple linear congruential generator for randomness
    push rbx            ; Save registers
    push r12
    push r13
    push r14
    mov r12, rdi        ; r12 = directions array
    mov r13, rsi        ; r13 = count
    mov r14d, edx       ; r14d = seed
    dec r13             ; Last index is count-1
.shuffle_loop:
    cmp r13, 0          ; Check if we've processed all elements
    jle .done           ; If we've processed everything, we're done
    ; Generate a random index between 0 and i
    ; Using LCG algorithm: next = (a*seed + c) % m
    mov eax, r14d       ; eax = seed
    imul eax, 1103515245
    add eax, 12345
    mov r14d, eax       ; Update seed
    ; Get random index j between 0 and i (inclusive)
    xor edx, edx        ; Clear high bits for division
    div r13d            ; edx = seed % (i+1)
    ; Swap directions[i] and directions[j]
    mov ebx, [r12 + r13*4]         ; ebx = directions[i]
    mov eax, [r12 + rdx*4]         ; eax = directions[j]
    mov [r12 + r13*4], eax         ; directions[i] = directions[j]
    mov [r12 + rdx*4], ebx         ; directions[j] = temp
    dec r13             ; Decrement counter
    jmp .shuffle_loop   ; Continue with next element
.done:
    pop r14             ; Restore registers
    pop r13
    pop r12
    pop rbx
    ret
