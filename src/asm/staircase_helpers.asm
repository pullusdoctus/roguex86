section .text
global _checkValidStaircasePosition    ; checks if position is valid for staircase
global _isDoorwayProximity             ; checks if position is near a doorway

; bool _checkValidStaircasePosition(int x, int y, int width, int height)
; Checks if a position is valid for placing a staircase
; Parameters:
;   x - X position to check (edi)
;   y - Y position to check (esi)
;   width - Room width (edx)
;   height - Room height (ecx)
; Returns:
;   1 if valid position, 0 if invalid
_checkValidStaircasePosition:
  ; Check if position is within room bounds (excluding walls)
  cmp edi, 0          ; x < 0?
  jle .invalid
  cmp esi, 0          ; y < 0?
  jle .invalid
  dec edx             ; width - 1 (last valid position)
  dec ecx             ; height - 1 (last valid position)
  cmp edi, edx        ; x >= width - 1?
  jge .invalid
  cmp esi, ecx        ; y >= height - 1?
  jge .invalid
  ; Check that we're not on a border tile (x > 0, y > 0)
  cmp edi, 1          ; x <= 1? (border)
  jle .invalid
  cmp esi, 1          ; y <= 1? (border)
  jle .invalid
  ; Valid position
  mov eax, 1
  ret
.invalid:
  xor eax, eax        ; Return 0 (false)
  ret

; bool _isDoorwayProximity(int x, int y, int doorwayX, int doorwayY, int proximityThreshold)
; Checks if position is too close to a doorway
; Parameters:
;   x - X position to check (edi)
;   y - Y position to check (esi)
;   doorwayX - X position of doorway center (edx)
;   doorwayY - Y position of doorway center (ecx)
;   proximityThreshold - Threshold distance (r8d)
; Returns:
;   1 if too close to doorway, 0 if safe
_isDoorwayProximity:
  ; Check if x is too close to doorwayX
  mov eax, edi
  sub eax, edx        ; x - doorwayX
  ; Get absolute value of (x - doorwayX)
  cmp eax, 0
  jge .check_x_upper
  neg eax             ; abs(x - doorwayX)
.check_x_upper:
  cmp eax, r8d        ; Compare with threshold
  jg .check_y         ; If > threshold, check Y
  ; X is within threshold, now check Y
.check_y:
  mov eax, esi
  sub eax, ecx        ; y - doorwayY
  ; Get absolute value of (y - doorwayY)
  cmp eax, 0
  jge .check_y_upper
  neg eax             ; abs(y - doorwayY)
.check_y_upper:
  cmp eax, r8d        ; Compare with threshold
  jg .not_in_proximity  ; If > threshold, not in proximity
  ; In proximity of doorway
  mov eax, 1
  ret
.not_in_proximity:
  xor eax, eax        ; Return 0 (false)
  ret
