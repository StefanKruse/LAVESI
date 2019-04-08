# set wd
#setwd("M:/Documents/Programmierung/git/LAVESI")
#setwd("N:/geonob1/BioIng/UserData/stkruse/Programming_LAVESI/")


folders=c("omp_transect_first")			
folders=c("omp_transect_first_8cores")	

		
printORplot="print"
for(foldi in folders)
{# plot all start

t_N_mort=read.table(paste0("./",foldi,"/","t_N_mort.txt"), sep=";")
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
				# end_time_Dataoutput - start_time_Dataoutput ,
				# end_time_mortalitaet - start_time_mortalitaet,
				# end_time_Ageing - start_time_Ageing ,
				# (end_time_Ageing - start_time_Ageing)+
					# (end_time_mortalitaet - start_time_mortalitaet)+
					# (end_time_Dataoutput - start_time_Dataoutput)+
					# (end_time_etablierung - start_time_etablierung)+
					# (end_time_Treeverteilung - start_time_Treeverteilung)+
					# (end_time_seedproduktion - start_time_seedproduktion)+
					# (end_time_seedausbreitung - start_time_seedausbreitung)+
					# (end_time_wachstum - start_time_wachstum)+
					# (end_time_kartenup - start_time_kartenup)
			# );
names(t_N_mort)=c("N_tree", "time", "mort", "kartenup", "wachstum", "seedausbreit", "seedprod", "treeverteilung", "etablierung", "dataoutput", "mortality", "ageing", "all")
# str(t_N_mort)



# plot
if(printORplot=="plot")
	dev.new()
else
	png(paste0("./",foldi,"/postprocessing/transect_analysis_computationtime__",foldi,".png"), width=800, height=800, res=100)

namei=foldi
# layout(matrix(c(1:(2*length(namesi))), ncol=2, nrow=length(namesi), byrow=TRUE), width=c(1,0.4), height=1)
# layout.show(2*length(namesi))
par(mfrow=c(2,2), las=2, mar=c(8,4,0,0))

	damat=t_N_mort[4:13]
	maxx=dim(damat)[1]#580
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

	
	
t_N_poll=read.table(paste0("./",foldi,"/","t_N_poll.txt"), sep=";")
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

	
	
	
	
	
t_N_plotupdate=read.table(paste0("./",foldi,"/","t_N_plotupdate.txt"), sep=";")
					# parameter[0].ivort, 
					# tree_list.size(),
					# time_ResetMaps-time_start_0,
					# time_AddTreeDensity-time_ResetMaps, 
					# omp_get_wtime()-time_AddTreeDensity
names(t_N_plotupdate)=c("time", "N_tree", "ResetMaps", "AddTreeDensity", "IndividualTreeDensity")
# str(t_N_plotupdate)

	# par(mfrow=c(1,2), las=2, mar=c(8,4,0,0))
	barplot(apply(t_N_plotupdate[,3:5],2,mean)/sum(apply(t_N_plotupdate[,3:5],2,mean)))

if(printORplot=="print")
	dev.off()	

}#plot all end



