# set wd
#setwd("M:/Documents/Programmierung/git/LAVESI")
#setwd("N:/geonob1/BioIng/UserData/stkruse/Programming_LAVESI/omp_transect_first")

{# POSITIONS
	for(fi in c(1,100,200,300,400))
	{
		dpos=read.csv2(paste0("./output/","dataentwicklung_positionsanalyse_1_22_",fi,".csv"), dec=".")
		# str(dpos)

		png(paste0("./postprocessing/transect_analysis__temporal",formatC(fi,width=3,flag="0"),".png"), height=3*1200, width=3*400, res=144)
		# pdf(paste0("./postprocessing/transect_analysis__temporal",formatC(fi,width=3,flag="0"),".pdf"), height=2*1200, width=2*400)#, res=144)
			par(mfrow=c(1,2))
			with(dpos, plot(Y~X, pch=16, main=fi, col=ifelse(Y>50000,"orange","forestgreen"), ylim=c(0,250000)))
				# focus auf grenze
			with(dpos[dpos$Y>50000-100,], plot(Y~X, pch=16, ylim=c(50000,51000)-100, main=fi, col=ifelse(Y>50000,"orange","forestgreen")))
		dev.off()
		
		rm(dpos)
	}


}


{# CURRENCIES
	dcur=read.csv2("./output/dataentwicklung_currencies_1_22.csv", dec=".")
	str(dcur)
	
	png("./postprocessing/transect_analysis__Number of individuals.png",height=2*400, width=2*1200, res=144)
		# dev.new(height=400, width=1200)
		par(mar=c(4,15,2,2), las=1)
		with(dcur, plot(NderH200bis10000~Fortschritt, type="l", lwd=2, col="forestgreen", yaxt="n", ylab=""))
			axis(2, line=0, at=axTicks(2), labels=axTicks(2)/1000000)
			mtext(side=2, line=3, "Number of trees [*1 000 000]", las=3, col="forestgreen")
		par(new=TRUE)
		with(dcur, plot(NderH40bis200~Fortschritt, type="l", lwd=2, col="orange", yaxt="n", ylab=""))
			axis(2, line=5, at=axTicks(2), labels=axTicks(2)/1000000)
			mtext(side=2, line=8, "Number of saplings [*1 000 000]", las=3, col="orange")
		par(new=TRUE)
		with(dcur, plot(NderH0bis40~Fortschritt, type="l", lwd=2, col="slateblue", yaxt="n", ylab=""))
			axis(2, line=10, at=axTicks(2), labels=axTicks(2)/1000000)
			mtext(side=2, line=13, "Number of seedlings [*1 000 000]", las=3, col="slateblue")
	dev.off()

}