setwd("/home/pdancak/school/MrPain_T/MUNI/MV013/HW1")
data <- read.csv("HousePrices.csv",header = TRUE ,sep=';',fileEncoding = "UTF-8-BOM")

print("1.a")
names(data)[names(data) == "X"] <- "Id"

print("1.b")
data$LotArea<-gsub(" m2", "", data$LotArea)
data$LotArea<-as.numeric(data$LotArea)
data$Id<-as.numeric(data$Id)
data$OverallCond<-as.numeric(data$OverallCond)
data$YearBuilt<-as.numeric(data$YearBuilt)
data$YearRemodAdd<-as.numeric(data$YearRemodAdd)
data$TotalBsmtSF<-as.numeric(data$TotalBsmtSF)
data$Fireplaces<-as.numeric(data$Fireplaces)
data$GarageCars<-as.numeric(data$GarageCars)
data$GarageArea<-as.numeric(data$GarageArea)
data$PoolArea<-as.numeric(data$PoolArea)
data$MoSold<-as.numeric(data$MoSold)
data$YrSold<-as.numeric(data$YrSold)
data$SalePrice<-as.numeric(data$SalePrice)

print("1.c")
data$CentralAir <- gsub("Y","Yes",data$CentralAir)
data$CentralAir <- gsub("N","No",data$CentralAir)
data$CentralAir <- factor(data$CentralAir)
table(data$CentralAir)
data <- subset(data, select = -LotShape)

print("1.d")
for(name in colnames(data)){
  num = length(data[name[1]][is.na(data[name[1]])])
  if(num > 0){
    print(paste("Column", name[1], "has", num, "of missing values"))
  }
}

print("Conclusion: No house has pool or the pool count wasn't measured, 159 houses don't have garage, 
       82 houses don't have basement and 1420 houses don't have fireplace. There is 2909
       missing values in pool quality, because these houses dont have pool.")
print("2.a")
my_mean <- function(x){
  return(sum(x)/length(x))
}

my_median <- function(x){
  x <- sort(x)
  n <- length(x)/2
  #if length is odd need to round to get int
  if(n != round(n)) return(x[round(n)])
  #even number doesn't have middle point so use average of 2 middle points
  return((x[n]+x[n+1])/2)
}

my_quantile <- function(x){
  x <- sort(x)
  n <- length(x)
  tmp0 <- x[1]
  tmp1 <- x[ceiling(n/4)]
  tmp2 <- x[ceiling(n/2)]
  tmp3 <- x[ceiling(0.75*n)]
  tmp4 <- x[n]
  my_vec <- c(tmp0, tmp1, tmp2, tmp3, tmp4)
  return(my_vec)
}

mean(data$Id)
my_mean(data$Id)

median(data$Id)
my_median(data$Id)

quantile(data$Id, type=1)
my_quantile(data$Id)

print("2.b")
my_mean(data$SalePrice[data$Fireplaces == 0])
length(data$SalePrice[data$Fireplaces == 0])
my_mean(data$SalePrice[data$Fireplaces == 1])
length(data$SalePrice[data$Fireplaces == 1])
my_mean(data$SalePrice[data$Fireplaces == 2])
length(data$SalePrice[data$Fireplaces == 2])
my_mean(data$SalePrice[data$Fireplaces == 3])
length(data$SalePrice[data$Fireplaces == 3])
print("Conclusion: the most expensive houses are the ones with the 2 fireplaces.
(conclusion can be bad because there is not enough data about houses with 3 fireplaces)")

print("2.c")
par(mfrow=c(1,2))
hist(data$LotArea, breaks = seq(0,8000000,by=5000), xlim=c(0, 200000))
hist(data$SalePrice, breaks = seq(0,8000000,by=5000), xlim=c(0, 200000))

print("Variability in LotArea is much lower than in SalePrice.
LotArea shape is positively skewed, SalePrice is also skewed positively, because of 0's.
Median for LotArea should be somewhere between 7500-10000 and for SalePrice it should be 125000-150000 (if we wont count 0's)")

print("LotArea median is lower than mean --> positive skew ----- median < mean")
my_median(data$LotArea)
my_mean(data$LotArea)

print("SalePrice with zeros
if we will count 0's the graph is positively skewed ----- median < mean")
my_median(data$SalePrice)
my_mean(data$SalePrice)

print("if we ignore 0's the graph is positively skewed but not that much as with 0 ----- median < mean")
my_median(data$SalePrice[data$SalePrice != 0])
my_mean(data$SalePrice[data$SalePrice != 0])

print("variability is much higher in SalePrice, but if we wont count 0's variability will be much lower than before")
var(data$LotArea)
var(data$SalePrice)
var(data$SalePrice[data$SalePrice != 0])

my_quantile(data$LotArea)
my_quantile(data$SalePrice)

print("2.d")

print("LandSlope is nominal data type and Heating is ordinal data type")
names <- c("Gtl", "Mod", "Sev")
n <- length(data$LandSlope)
percentage <- c(round(100*length(data$LandSlope[data$LandSlope == "Gtl"])/n))
percentage <- append(percentage, c(round(100*length(data$LandSlope[data$LandSlope == "Mod"])/n)))
percentage <- append(percentage, c(round(100*length(data$LandSlope[data$LandSlope == "Sev"])/n)))
class(data$LandSlope)
x = c(length(data$LandSlope[data$LandSlope == "Gtl"]), length(data$LandSlope[data$LandSlope == "Mod"]),
      length(data$LandSlope[data$LandSlope == "Sev"]))
pie(table(data$LandSlope), main="Pie chart of LandSlope", labels = paste(names, percentage, "%"))

names <- c("Floor", "GasA", "GasW", "Grav", "OthW", "Wall")
n <- length(data$Heating)
percentage <- c(round(100*length(data$Heating[data$Heating == "Floor"])/n))
percentage <- append(percentage, c(round(100*length(data$Heating[data$Heating == "GasA"])/n)))
percentage <- append(percentage, c(round(100*length(data$Heating[data$Heating == "GasW"])/n)))
percentage <- append(percentage, c(round(100*length(data$Heating[data$Heating == "Grav"])/n)))
percentage <- append(percentage, c(round(100*length(data$Heating[data$Heating == "OthW"])/n)))
percentage <- append(percentage, c(round(100*length(data$Heating[data$Heating == "Wall"])/n)))
pie(table(data$Heating), main="Pie chart of LandSlope", labels = paste(names, percentage, "%"), cex = 0.5)
print("No we cant use median for measure of location, because we cant order the data in table, but we can
use the mode (most common values)")

print("3.a")
my_var <- function(x){
  n <- length(x)
  meann <- my_mean(x)
  varr <- 0
  for(i in x){
    varr <- (i - meann)^2 + varr
  }
  return(varr/n)
}
my_cov <- function(x, y){
  sum <- 0
  E_x <- my_mean(x)
  E_y <- my_mean(y)
  n <- length(x)
  for(i in 1:n){
    sum <- sum + (x[i] - E_x) * (y[i] - E_y)
  }
  return(sum/(n))
}
my_correlation <- function(x, y){

  n <- length(x)
  E_x <- my_mean(x)
  E_y <- my_mean(y)
  #cov(X,Y)/sqrt(varX*varY)
  cor_xy <- my_cov(x, y)/sqrt(my_var(x)*my_var(y))
  return(cor_xy)
}

print("3.b")
print("NA value in house from 1923 (garagecars and garagearea)")
table(data$GarageCars[data$YearBuilt > 1920 | data$YearBuilt < 1930]) #median is 2
data$GarageCars[is.na(data$GarageCars) == TRUE] <- my_median(data$GarageCars[data$YearBuilt > 1920 | data$YearBuilt < 1930])
data$GarageArea[is.na(data$GarageArea) == TRUE] <- my_median(data$GarageArea[data$YearBuilt > 1920 | data$YearBuilt < 1930])

print("NA value in house from 1946")
data$TotalBsmtSF[is.na(data$TotalBsmtSF) == TRUE] <- my_median(data$TotalBsmtSF[data$YearBuilt > 1940 | data$YearBuilt < 1950])

print("3.c)")
my_correlation(data$SalePrice, data$LotArea)
my_correlation(data$SalePrice, data$TotalBsmtSF)
my_correlation(data$SalePrice, data$Id)
my_correlation(data$SalePrice, data$OverallCond)
my_correlation(data$SalePrice, data$YearBuilt)
my_correlation(data$SalePrice, data$YearRemodAdd)
my_correlation(data$SalePrice, data$Fireplaces)
my_correlation(data$SalePrice, data$GarageCars)
my_correlation(data$SalePrice, data$GarageArea)
my_correlation(data$SalePrice, data$PoolArea)
my_correlation(data$SalePrice, data$MoSold)
my_correlation(data$SalePrice, data$YrSold)
print("most linear dependency is between SalePrice, (TotalBsmtSF, ID, GarageCars, [GarageArea] if ID wont count)")

print("#3.d")
my_var(as.numeric(as.factor(data$LandSlope)))
my_var(as.numeric(as.factor(data$HouseStyle)))
my_var(as.numeric(as.factor(data$Heating)))
my_var(as.numeric(as.factor(data$HeatingQC)))
my_var(as.numeric(as.factor(data$CentralAir)))
table(data$Heating)
table(data$HouseStyle)
print("Conclusion: Lowest variance is in heating. It is because most houses use gas for heating.
            Highest variance is in house style. Houses can look different and there is no
such standard, which house is better like in heating. Also, there are much more categories than in heating.")