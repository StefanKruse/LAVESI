## read origi tree distri
# ... rewrite to XX * XX size and fill all spaces
setwd("M:/Documents/Programmierung/git/LAVESI/input")
tredat=read.table("CH17I_Treevert2011.csv", sep=";", dec=".", header=TRUE)
str(tredat)
# data is already of all trees>100 years and stretched over 100x100 m

# write.table(tredat[,2:8], "CH17I_Treevert2011_100_100.csv", sep=";", dec=".", quote=FALSE)


{############## <-<- QUADRATIC
# for(expan in c(5,10))#,20,50))# for each 100*
for(expan in c(20))#,20,50))# for each 100*
{
trnew=NULL
# expan=5
for(xei in 1:(expan-1))
{
for(yei in 1:(expan-1))
{
	id=tredat[,2:8]
	id$Y=id$Y+100*yei
	id$X=id$X+100*xei
	trnew=rbind(trnew, id)
}
}
# str(trnew)
# with(trnew, plot(Y~X, cex=1+Hoehe/max(trnew$Hoehe)))

write.table(trnew, paste0("CH17I_Treevert2011_",100*expan,"_",100*expan,".csv"), sep=";", dec=".", quote=FALSE)

}

# set.seed(0)
# with(tredat, plot(jitter(Y,100)~jitter(X,100)))
# with(tredat, plot(Y~X, cex=1+Hoehe/max(tredat$Hoehe)))
# with(tredat, plot(Y~X, cex=1+Hoehe/max(tredat$Alter)))
}





{############## <-<- RECTANGLES==TRANSECTS
x_expan=1 ## in hundreds
y_expan=500 ## in hundreds

trnew=NULL
# expan=5
for(xei in seq(from=0, to=(x_expan-1), by=1))
{
for(yei in seq(from=0, to=(y_expan-1), by=1))
{
	print(paste0(xei,"/",yei))
	id=tredat[,2:8]
	id$Y=id$Y+100*yei
	id$X=id$X+100*xei
	trnew=rbind(trnew, id)
}
}
str(trnew)
with(trnew, plot(Y~X, cex=1+Hoehe/max(trnew$Hoehe)))

write.table(trnew, paste0("CH17I_Treevert2011_",100*x_expan,"_",100*y_expan,".csv"), sep=";", dec=".", quote=FALSE)

}



