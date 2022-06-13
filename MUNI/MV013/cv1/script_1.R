comp <- read.csv("Computers_cleaning.csv",sep=';',fileEncoding = "UTF-8-BOM")

comp$price[is.na(as.numeric(comp$price))]
comp$price<-gsub(",","",comp$price)

#1.
comp$speed[is.na(as.numeric((comp$speed)))]

comp$hd[is.na(as.numeric((comp$hd)))]
comp$ram[is.na(as.numeric((comp$ram)))]
comp$screen[is.na(as.numeric((comp$screen)))]
comp$ads[is.na(as.numeric((comp$ads)))]
comp$trend[is.na(as.numeric((comp$trend)))]

#2.
comp$cd[comp$cd != "no" & comp$cd != "yes"]
comp$cd<-gsub("Yes", "yes",comp$cd)
comp$cd<-gsub("Y", "yes",comp$cd)
comp$cd<-gsub("N", "no",comp$cd)
comp$cd<-gsub("na", NA,comp$cd)
comp$cd[comp$cd == ""]<- NA

comp$multi[comp$multi != "no" & comp$multi != "yes"]
comp$multi<-gsub("Yes", "yes",comp$multi)
comp$multi<-gsub("N", "no",comp$multi)
comp$multi<-gsub("na", NA,comp$multi)
comp$multi[comp$multi == ""]<- NA
comp$multi[comp$multi == "noo" | comp$multi == "noO"]<-"no"

comp$premium[comp$premium != "no" & comp$premium != "yes"]
comp$premium[comp$premium == "N"] <- "no"
comp$premium[comp$premium == "Y" | comp$premium == "Yes" | comp$premium == "YES"] <- "yes"
comp$premium[comp$premium == "na" | comp$premium == ""] <- NA

#3.
comp$cd[comp$cd == "yes"] <- TRUE
comp$cd[comp$cd == "no"] <- FALSE
class(comp$cd) <- "logical"

comp$multi[comp$multi == "yes"] <- TRUE
comp$multi[comp$multi == "no"] <- FALSE
class(comp$multi) <- "logical"

table(comp$premium)
comp$premium[comp$premium == "yes"] <- TRUE
comp$premium[comp$premium == "no"] <- FALSE
class(comp$premium) <- "logical"

#4.
comp$ram <- factor(comp$ram)
levels(comp$ram)

#5.
table(comp$premium)
table(comp$ram)
table(comp$premium, comp$ram)

#6.
comp$price_range[comp$price < 3000] <-"normal"
comp$price_range[comp$price >= 3000] <-"expensive"
table(comp$price_range)x\
table(comp$price_range, comp$premium)

str(comp)
