## read origi tree distri
# ... rewrite to XX * XX size and fill all spaces
setwd("M:/Documents/Programmierung/git/LAVESI/input")
tredat=read.table("CH17I_Treevert2011.csv", sep=";", dec=".", header=TRUE)
str(tredat)



# set.seed(0)
# with(tredat, plot(jitter(Y,100)~jitter(X,100)))
# with(tredat, plot(Y~X, cex=1+Hoehe/max(tredat$Hoehe)))
# with(tredat, plot(Y~X, cex=1+Hoehe/max(tredat$Alter)))


# data is already of all trees>100 years and stretched over 100x100 m



