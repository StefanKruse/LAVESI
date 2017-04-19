### --- Code for computation time analyses when multi-core processing --- ###




### set paths for working on differen platforms
	gitcodepathpostprocess="/home/stefan/Desktop/Programming/git/LAVESI/postprocessing/" 	# Stefan-Work-PC2


### data read in 
	folderstoanalyse=c(
	  "/home/stefan/Desktop/Programming/git/LAVESI/TEST/core2",
	  "/home/stefan/Desktop/Programming/git/LAVESI/TEST/core2_splice"
		)
		
	timedf=NULL
	curtimedf=NULL

	for(i in 1:length(folderstoanalyse))
	{
		# computation times
			tnall=read.table(paste0(folderstoanalyse[i], "/", "t_N.txt"))
			names(tnall)=c("TreeListSize", "T_all")
			tnmort=read.table(paste0(folderstoanalyse[i], "/", "t_N_mort.txt"), sep=";")
			names(tnmort)=c("TreeListSize", "Ivort", "T_mort")
			tnmerge=cbind(Cores=rev(strsplit(folderstoanalyse[[i]], split="/")[[1]])[1],tnmort, tnall)
			# store in container
			timedf=rbind(timedf, tnmerge)
		
		# currencies
			cur=read.csv2(paste0(folderstoanalyse[i], "/output/", "dataentwicklung_currencies_1_24.csv"))
			names(cur)[which(names(cur)=="Fortschritt")]="Ivort"
			curmerge=merge(cur, tnmerge, sort=FALSE)
			curmerge=cbind(Cores=rev(strsplit(folderstoanalyse[[i]], split="/")[[1]])[1], curmerge)
			# store in container
			curtimedf=rbind(curtimedf, curmerge)
	}
	str(timedf)
	summary(timedf)



### data analyses
	#png(paste0(gitcodepathpostprocess, "comptime.png"))
		with(timedf, plot(T_mort~TreeListSize, log="xy", col=Cores, pch=ifelse(Ivort<100,15, 1)))
			title(main="Computation time")
			legend("topleft", c(levels(timedf$Cores), "ivort<100", "ivort>=100"), col=c(1:length(levels(timedf$Cores)),1,1), pch=c(rep(1,length(folderstoanalyse)), 15,1), pt.cex=2, pt.lwd=3)
	#dev.off()


  
  
	with(timedf[timedf$TreeListSize>7500,], plot(T_mort~TreeListSize, log="xy", col=Cores, pch=ifelse(Ivort<100,15, 1)))
	title(main="Computation time")
	legend("topleft", c(levels(timedf$Cores), "ivort<100", "ivort>=100"), col=c(1:length(levels(timedf$Cores)),1,1), pch=c(rep(1,length(folderstoanalyse)), 15,1), pt.cex=2, pt.lwd=3)
  
  # ablines for different runs
  for(lvli in levels(timedf$Cores))
  {
    lm1=with(timedf[timedf$TreeListSize>7500 & timedf$Cores==lvli,], lm(T_mort~TreeListSize))
    str(lm1)
    ypred=lm1$fitted.values
    xpred=lm1$model$TreeListSize
    ypred=ypred[order(xpred)]
    xpred=xpred[order(xpred)]
    lines(y=ypred, x=xpred, col=which(levels(timedf$Cores)==lvli))
  }












