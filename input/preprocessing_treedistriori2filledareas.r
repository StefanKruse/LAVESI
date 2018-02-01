## read original tree distribution
# ... rewrite to XX * XX size and fill all spaces

# new files must be registered in 




# set working directory
setwd(choose.dir())# select current folder of this file

# read raw data
tredat=read.table("CH17I_Treevert2011.csv", sep=";", dec=".", header=TRUE)
str(tredat)
# data is already of all trees>100 years and stretched over 100x100 m


# create new distribution files
{############## <-<- QUADRATIC
	for(expan in 20)# in hundreds
	{
		trnew=NULL
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

		write.table(trnew, paste0("CH17I_Treevert2011_",100*expan,"_",100*expan,".csv"), sep=";", dec=".", quote=FALSE)
	}
}





{############## <-<- RECTANGLES==TRANSECTS
	x_expan=1 ## in hundreds
	y_expan=500 ## in hundreds

	trnew=NULL
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



