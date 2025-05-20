global rand_between:

; int rand_between(int64_t lower_bound, int64_t upper_bound)
rand_between:
  ; store the bounds in registers
  push rbp
  mov rbp, rsp
  push rbx
  push rcx
  push rdx
  push r8
  push r9
  push r10
  ; get the bounds
  mov r8, rdi  ; lower_bound
  mov r9, rsi  ; upper_bound

; gen random seed
.try_rdseed:
  rdseed rax
  jnc .try_rdseed  ; retry if not succesful

  ; gen pseudo-random value w/ xorshift64
  mov rbx, rax
  mov r10, rbx
  shr r10, 12
  xor rbx, r10
  mov r10, rbx
  shr r10, 27
  xor rbx, r10
  mov rax, 2685821657736338717
  imul rbx, rax ; multiply by a constant for xorshift
  mov rax, rbx  ; use the generated random value
  ; reduce value to range
  mov rcx, r9   ; rcx = upper_bound
  sub rcx, r8   ; rcx = upper_bound - lower_bound
  inc rcx       ; rcx = (upper_bound - lower_bound) + 1 = range
  xor rdx, rdx  ; clear rdx for division
  div rcx       ; rax / rcx, quotient in rax, remainder in rdx
  mov rax, rdx  ; rax = remainder (0 <= r < range)
  add rax, r8   ; shift to range

  ; restore registers
  pop r10
  pop r9
  pop r8
  pop rdx
  pop rcx
  pop rbx
  pop rbp
  ret
