
setwd("M:/Documents/Programmierung/git/LAVESI")

{# plot all start
t_N_mort=read.table("t_N_mort.txt", sep=";")
# str(t_N_mort)
			# tree_list.size(),
			# parameter[0].ivort, 
			# end_time_mortalitaet - start_time_mortalitaet,
			# end_time_kartenup - start_time_kartenup ,// from here newly introduced to record all
				# end_time_wachstum - start_time_wachstum ,
				# end_time_seedausbreitung - start_time_seedausbreitung,
				# end_time_seedproduktion - start_time_seedproduktion,
				# end_time_Treeverteilung - start_time_Treeverteilung,
				# end_time_etablierung - start_time_etablierung,
				# end_time_feuer - start_time_feuer,
				# end_time_Dataoutput - start_time_Dataoutput ,
				# end_time_mortalitaet - start_time_mortalitaet,
				# end_time_Ageing - start_time_Ageing ,
				# (end_time_Ageing - start_time_Ageing)+
					# (end_time_mortalitaet - start_time_mortalitaet)+
					# (end_time_Dataoutput - start_time_Dataoutput)+
					# (end_time_feuer - start_time_feuer)+
					# (end_time_etablierung - start_time_etablierung)+
					# (end_time_Treeverteilung - start_time_Treeverteilung)+
					# (end_time_seedproduktion - start_time_seedproduktion)+
					# (end_time_seedausbreitung - start_time_seedausbreitung)+
					# (end_time_wachstum - start_time_wachstum)+
					# (end_time_kartenup - start_time_kartenup)
			# );
names(t_N_mort)=c("N_tree", "time", "mort", "kartenup", "wachstum", "seedausbreit", "seedprod", "treeverteilung", "etablierung", "feuer", "dataoutput", "mortality", "ageing", "all")
# str(t_N_mort)







# plot
namei="16core500"
# layout(matrix(c(1:(2*length(namesi))), ncol=2, nrow=length(namesi), byrow=TRUE), width=c(1,0.4), height=1)
# layout.show(2*length(namesi))
par(mfrow=c(2,2), las=2, mar=c(8,4,0,0))
maxx=dim(damat)[1]#580

	damat=t_N_mort[4:13]
	rm=apply(damat,1,sum)
	for(rowi in 1:dim(damat)[1])
	{
		damat[rowi,]=damat[rowi,]/rm[rowi]
	}
	clbp=rainbow(s=0.5,n=dim(damat)[2])
	
		# add nas for missing
		dfiad=as.data.frame(matrix(NA, nrow=maxx-dim(damat)[1], ncol=dim(damat)[2]))
		names(dfiad)=names(damat)
		damat=rbind(damat,dfiad)
		
	par(mar=c(4,0,1,0))

	barplot(t(as.matrix(damat)), border=NA, yaxt="n", col=clbp, axes=FALSE, names.arg=rep(NA,maxx))
	title(main=paste0(namei, " -> mean time for one year (sim years=", max(t_N_mort$time),"): ",round(mean(rm),0)," sec"),cex.main=0.7)
		# overlay full time for computation
		par(new=TRUE)
		with(t_N_mort, plot(all~time, type="l", lwd=2, xaxt="n", yaxt="n", ylab="", xlab="", bty="n", xlim=c(0,maxx)))

	par(mar=c(8,0,1,3))
	
	# barplot(apply(damat[1:dim(t_N_mort[4:13])[1],],2,mean), border=NA, yaxt="n", las=2, col=clbp)
	# axis(side=4,las=1)
	
	par(mar=c(8,0,1,3))
	barplot(apply(t_N_mort[4:13][1:dim(t_N_mort[4:13])[1],],2,mean), border=NA, yaxt="n", las=2, col=clbp)
	axis(side=4,las=1)
	mtext(side=4, "comp. time [s]",las=3, line=-1)

	
	
t_N_poll=read.table("t_N_poll.txt", sep=";")
					# parameter[0].ivort, 
					# seed_list.size(),
					# tree_list.size(),
					# (end_time_poll - end_time_seedsuviving), // pollination total
					# (end_time_mortpoll - end_time_poll), // only tree mortality
					# (end_time_seedsuviving - start_time_mortpoll), // seed mortality
					# //timers each tree
					# timer_eachtree_advance_all,
					# timer_eachtree_vectini_all,
					# timer_eachtree_seedsurv_all,
					# timer_eachtree_seedadd_all,
					# timer_eachtree_total_all
names(t_N_poll)=c("time", "N_seed", "N_tree", "pollination", "treemortality", "seedmortality", "P_advance", "P_vectini", "seedsurv", "seedadd", "total")
# str(t_N_poll)

	# par(mfrow=c(1,2), las=2, mar=c(8,4,0,0))
	barplot(apply(t_N_poll[,4:6],2,mean)/sum(apply(t_N_poll[,4:6],2,mean)))
	# barplot(apply(t_N_poll[,7:10],2,mean)/sum(apply(t_N_poll[,7:10],2,mean)))
	# barplot(apply(t_N_poll[,7:11],2,mean)/sum(apply(t_N_poll[,7:11],2,mean)))

	
	
	
	
	
t_N_plotupdate=read.table("t_N_plotupdate.txt", sep=";")
					# parameter[0].ivort, 
					# tree_list.size(),
					# time_ResetMaps-time_start_0,
					# time_AddTreeDensity-time_ResetMaps, 
					# omp_get_wtime()-time_AddTreeDensity
names(t_N_plotupdate)=c("time", "N_tree", "ResetMaps", "AddTreeDensity", "IndividualTreeDensity")
# str(t_N_plotupdate)

	# par(mfrow=c(1,2), las=2, mar=c(8,4,0,0))
	barplot(apply(t_N_plotupdate[,3:5],2,mean)/sum(apply(t_N_plotupdate[,3:5],2,mean)))

}#plot all end


	
	
	


	
	
	
	
	
	
	
	



#### 31.5. first one
t_N_poll=read.table("t_N_poll.txt", sep=";")
names(t_N_poll)=c("t", "N_seed", "N_tree", "pollination", "treemortality", "seedmortality")

png("./postprocessing/computationtime_realsecondsOMP_absolute-first10years.png")
	# with(t_N_poll[as.numeric(row.names(t_N_poll))<=10,], plot(pollination~t, type="b", ylim=c(0,with(t_N_poll,max(c(pollination,treemortality,seedmortality)))), xlim=c(0,10)))
	with(t_N_poll[as.numeric(row.names(t_N_poll))<=10,], plot(pollination~t, type="b", ylim=c(0,200), xlim=c(0,10), main="500x500 sqm area"))
	with(t_N_poll[as.numeric(row.names(t_N_poll))<=10,], points(treemortality~t, type="b", col="red"))
	with(t_N_poll[as.numeric(row.names(t_N_poll))<=10,], points(seedmortality~t, type="b", col="orange"))

	with(t_N_poll[as.numeric(row.names(t_N_poll))>10 & as.numeric(row.names(t_N_poll))<=19,], points(pollination~t, type="b", col="blue"))
	with(t_N_poll[as.numeric(row.names(t_N_poll))>10 & as.numeric(row.names(t_N_poll))<=19,], points(treemortality~t, type="b", col="red"))
	with(t_N_poll[as.numeric(row.names(t_N_poll))>10 & as.numeric(row.names(t_N_poll))<=19,], points(seedmortality~t, type="b", col="orange"))


	with(t_N_poll[as.numeric(row.names(t_N_poll))>19,], points(pollination~t, type="b", col="green"))
	with(t_N_poll[as.numeric(row.names(t_N_poll))>19,], points(treemortality~t, type="b", col="red"))
	with(t_N_poll[as.numeric(row.names(t_N_poll))>19,], points(seedmortality~t, type="b", col="orange"))

	legend("topleft", pch=1, col=c("black", "blue", "green"), c("Base","8 cores", "8 cores + pollination"), title="mortality.cpp: Pollination and Seed initialization:")

dev.off()





png("./postprocessing/computationtime_realsecondsOMP_relative.png")
barplot(matrix(c(as.vector(apply(t_N_poll[1:10,4:6],2,mean)/sum(apply(t_N_poll[1:10,4:6],2,mean))),as.vector(apply(t_N_poll[11:19,4:6],2,mean)/sum(apply(t_N_poll[11:19,4:6],2,mean))),as.vector(apply(t_N_poll[20:29,4:6],2,mean)/sum(apply(t_N_poll[20:29,4:6],2,mean)))),byrow=FALSE,ncol=3, nrow=3), names.arg=c("Base", "8 cores", "8 cores + pollination"), legend.text=c("polli","treemort","seedmort"), border=NA)
dev.off()
