setwd("/home/pdancak/school/MrPain_T/MUNI/MV013/cv4")

##1
#a
#hustota funkce /distribution
x <- c(seq(0, 100, by=1))
y <- dbinom(x, 100, 0.5)

plot(x,y)

#distribucni funkce
x3 <- pbinom(x, 100, 0.5)
plot(x3, type= 'l')


#b
x2 <- rbinom(1000, 100, 0.5)
plot(table(x2))
hist(x2, freq = FALSE)

#c
zeros_in_file<-function(fileName){
  data<-readChar(fileName, file.info(fileName)$size)
  data.int<-as.numeric(strsplit(data, "")[[1]])
  
  len<-100
  len_total<-length(data.int)
  nzeros<-c()
  for (i in seq(1,(len_total),len)){
    nzeros <- c(sum(data.int[i:(len+i)]),nzeros)
  }
  return(nzeros)}

zeros <- zeros_in_file("file.txt")
zeros2 <- zeros_in_file("file2.txt")
par(mfrow=c(1,2))
hist(zeros, freq = FALSE, ylim = c(0, 0.1))
hist(zeros2, freq = FALSE, ylim = c(0, 0.1))
par(mfrow=c(1,1))
##2

#a
poisson_process<-function(lambda,tt){
  all_t<-c()
  t<-0
  #count<-0
  while (t<tt)
  {
    current_time<-rexp(1,rate=lambda)
    t<-t+current_time
    #count<-count+1
    all_t<-c(all_t,t)
  }
  all_t<-all_t[1:length(all_t-1)]
  return(all_t)
}

time <- poisson_process(3, 100)
plot(time,0:(length(time)-1), type = 'l', ylim = c(0, 300), ylab= "num of events")

#b
par(mfrow=c(1,2))
run_index <- 1:500
cumulative_times <- run_index
for(i in run_index){
  time <- poisson_process(3, 100)
  cumulative_times[i] <- length(time)
}
hist(cumulative_times, freq = FALSE)

#c
time <- dpois(200:400, 3*100)
par(mfrow=c(1,1))


##3
x <- rbinom(1000, 100, 0.5)
