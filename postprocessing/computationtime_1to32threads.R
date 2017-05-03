## time consumtion 1-32 threads trial
# ... works ONLY with output of commited version at 21st April by Stefan Kruse at 4.30 pm when switching the if-logic answer at line 465 to true
# ... output file is included


# data input
setwd("M:/Documents/Programmierung/git/LAVESI/")
if(file.exists("postprocessing/computationtime_1to32threads.csv.gz"))
{
	ctimag=read.csv2("postprocessing/computationtime_1to32threads.csv.gz")
} else
{
	ctim=read.table("t_N_comptime.txt", header=FALSE, sep=";", dec=".")

	names(ctim)=c("N_tree", "Ivort", "threads", "t", "N_newSeeds")
	str(ctim)


	# plot
	with(ctim[ctim$threads==1,], plot(t~N_tree, pch=21, bg="gray", cex=2+N_newSeeds/max(ctim$N_newSeeds)))
	with(ctim[ctim$threads==1,], text(y=t, x=N_tree, 1 ))
	for(threadi in 2:32)
	{
		# with(ctim[ctim$threads==threadi,], text(t~N_tree, pch=21, bg="orange", cex=0.5+N_newSeeds/max(ctim$N_newSeeds)))
		with(ctim[ctim$threads==threadi,], text(y=t, x=N_tree, threadi ))
	}


	ctimag=NULL
	for(treesi in unique(ctim$N_tree))
	{
		for(threadi in 2:32)
		{
			ctimag=rbind(ctimag, data.frame(NT=treesi, threads=threadi, ct=mean(ctim[ctim$N_tree==treesi & ctim$threads==threadi,]$t/ctim[ctim$N_tree==treesi & ctim$threads==1,]$t)))
		}
	}
	str(ctimag)
	
	
	# save data
	write.csv2(ctimag, "postprocessing/computationtime_1to32threads.csv")
	library(R.utils)
	gzip("postprocessing/computationtime_1to32threads.csv", overwrite=TRUE)
}

# plot
pdf("postprocessing/computationtime_1to32threads.pdf")
	with(ctimag, boxplot(ct~threads, col="orange", log="y", main="relative computation speed compared to linear computation", ylab="rel. comp. time", xlab="Number of threads"))
dev.off()




