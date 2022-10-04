setwd("/home/pdancak/school/MrPain_T/MUNI/MV013/cv_3_4")
data <- read.csv("nama_10_a10_e_1_Data.csv",header = TRUE ,sep=',')

head(tt)
data <- data[ data$GEO != "European Union - 28 countries (2013-2020)" & data$GEO != "Euro area - 19 countries  (from 2015)",]
tt <- reshape(data[,c("GEO","NACE_R2_LABEL","Value")], direction="wide",idvar = "GEO", timevar = "NACE_R2_LABEL")

cor.matrix <- cor(tt[2:12])
cov.matrix <- cov(tt[2:12])
#install.packages("corrplot")
library(corrplot)
corrplot(cor.matrix)

eigenvalues <- eigen(cor.matrix)
