# r script for quick simulation check

# read files
	read.csv2("//dmawi/potsdam/data/bioing/user/stkruse/ChukotkaBiomassSims2020/LAVESI/output/dataentwicklung_currencies_1_2300851.csv")
	curs=read.csv2("//dmawi/potsdam/data/bioing/user/stkruse/ChukotkaBiomassSims2020/LAVESI/output/datatrees_currencies_2300851.csv", dec=".")
	str(curs)
	
# plot
## stemcout per time per elevation
	# ... aktort == YPLOTPOS
			{
				current_elevation = parameter[0].elevationoffset - 1000.00;
			} else if(aktort == 2)
			{
				current_elevation = parameter[0].elevationoffset - 500.00;
			} else if(aktort == 3)
			{
				current_elevation = parameter[0].elevationoffset - 50.00;
			} else if(aktort == 4)
			{
				current_elevation = parameter[0].elevationoffset - 0.00;
			} else if(aktort == 5)
			{
				current_elevation = parameter[0].elevationoffset + 50.00;
			} else if(aktort == 6)
			{
				current_elevation = parameter[0].elevationoffset + 500.00;
			} else if(aktort == 7)
			{
				current_elevation = parameter[0].elevationoffset + 1000.00;
			} else if(aktort == 8)
			{
				current_elevation = parameter[0].elevationoffset + 1500.00;
eles=c(-1000,-500,-50,0,+50,+500,+1000,+1500)
par(mfrow=c(8,1), mar=c(0,0,0,0),oma=c(4,4,0,0))
for(ypi in rev(unique(curs$YPLOTPOS)))
{
	with(curs[curs$YPLOTPOS==ypi,], plot(Stemcount~Year, type="l", ylim=c(0,160),lwd=2))
	abline(v=2018,lty=2,col="gray")
	text(y=75,x=1900,eles[ypi+1], col="blue", font=2)
}
mtext(outer=TRUE,side=1,"Year",line=2.5)
mtext(outer=TRUE,side=2,"Stemcount (#/ha)",line=2.5)












