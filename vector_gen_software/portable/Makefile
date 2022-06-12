isqrt_test : isqrt_test.o isqrt.o

EXAMPLES = 10 100 1000 10000 100000 1000000 2147483648 1073741824 200000000 300000000 400000000 500000000

test : isqrt_test
	for x in $(EXAMPLES) ; do \
		echo "input = $$x"; \
		r=`./isqrt_test $$x` ; \
		echo "root  = $$r"; \
		let sq=$$r*$$r; \
		echo "square= $$sq"; \
		echo ; \
	done
