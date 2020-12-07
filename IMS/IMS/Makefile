VAGONS=$(vagony)
TIME=$(cas)

ARG1="--vagony=$(vagony)"
ARG2="--cas=$(cas)"

ifndef VAGONS
ARG1=
endif
ifndef TIME
ARG2=
endif

run: model
	./model $(ARG1) $(ARG2)

model: model.cc
	g++ -g -O2 -lsimlib -lm -o model model.cc

clean:
	rm model