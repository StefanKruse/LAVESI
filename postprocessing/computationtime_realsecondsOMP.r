
setwd("M:/Documents/Programmierung/git/LAVESI")

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
