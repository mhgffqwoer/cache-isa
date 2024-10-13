addi    s11, zero, 64	// M = 64
addi    s10, zero, 60	// N = 60
addi    s9, zero, 32	// K = 32
lui     s0, 0x100		// pa
srli    s0, s0, 12
lui     s1, 0x1800		// pc
srli    s1, s1, 12
addi    t0, zero, 0		// y = 0
addi    t1, zero, 0		// x = 0
lui     s2, 0x900		// pb
srli    s2, s2, 12
addi    s3, zero, 0		// s = 0
addi    t2, zero, 0		// k = 0
add     t3, s0, t2		// pa[k] = k + pa
slli    t4, t1, 1		// x * 2
add     t4, t4, s2		// pb[x] = (x * 2) + pb	
lb      s4, 0, t3		// load
lh      s5, 0, t4		// load
mul     t3, s4, s5		// (sum) = pa[k] * pb[x]
add     s3, s3, t3		// s += (sum)
addi    s2, s2, 120		// pb += N
addi    t2, t2, 1		// ++k
blt     t2, s9, -36		// jump to string 14
slli	t3, t1, 2		// x * 4
add     t3, t3, s1		// pc[x] = (x * 4) + pc
sw      s3, 0, t3		// store
addi    t1, t1, 1		// ++x
blt     t1, s10, -72	// jump to string 10
addi    s0, s0, 32		// pa += K
addi    s1, s1, 240		// pc += N
addi    t0, t0, 1		// ++y
blt     t0, s11, -92	// jump to string 9
jalr    zero, ra, 0		// exit(0)