library(maps)
library(ggplot2)
world <- map_data("world")

if(file.exists("task_1.csv")){
	dat <- read.csv("task_1.csv",sep=",",header=TRUE)

	worldmap <- ggplot(world, aes(long, lat))
	worldmap <- worldmap + geom_polygon(fill = "#ffeda0", colour = "black",aes(group = group))
	worldmap <- worldmap + scale_x_continuous(name="Longitude",expand=c(0,0),limits=c(-180,191),breaks=seq(-180,180,10))
	worldmap <- worldmap + scale_y_continuous(name="Latitude",expand=c(0,0),breaks=seq(-90,90,10))
	worldmap <- worldmap + theme_bw() + theme(panel.background = element_rect(fill="#A3CCFF"))
	worldmap <- worldmap + theme(panel.grid.major=element_line(colour = "white"))
	worldmap <- worldmap + theme(panel.grid.minor=element_line(colour = "grey95", size = 0.25))
	worldmap <- worldmap + geom_point(data=dat,aes(x=longitude,y=latitude,size=31.62278^magnitude),colour="red")
	worldmap <- worldmap + scale_size(name="Magnitude",limits=c(31.62278^3.0,31.62278^9.5),breaks=c(31.62278^3.99,31.62278^4.99,31.62278^5.99,31.62278^6.99,31.62278^7.99,31.62278^8.99,31.62278^9.5),labels=c(3,4,5,6,7,8,9))

	pdf(file="task_1.pdf",width=15,height=6)
	print(worldmap)
	dev.off()
}

if(file.exists("task_2.csv")){
	dat <- read.csv("task_2.csv",sep=",",header=TRUE)

	worldmap <- ggplot(world, aes(long, lat))
	worldmap <- worldmap + geom_polygon(fill = "#ffeda0", colour = "black",aes(group = group))
	worldmap <- worldmap + scale_x_continuous(name="Longitude",expand=c(0,0),limits=c(-180,191),breaks=seq(-180,180,10))
	worldmap <- worldmap + scale_y_continuous(name="Latitude",expand=c(0,0),breaks=seq(-90,90,10))
	worldmap <- worldmap + theme_bw() + theme(panel.background = element_rect(fill="#A3CCFF"))
	worldmap <- worldmap + theme(panel.grid.major=element_line(colour = "white"))
	worldmap <- worldmap + theme(panel.grid.minor=element_line(colour = "grey95", size = 0.25))
	worldmap <- worldmap + geom_point(data=dat,aes(x=longitude,y=latitude,size=score),colour="red")
	worldmap <- worldmap + scale_size(name="score",range = c(2, 10))

	pdf(file="task_2.pdf",width=15,height=6)
	print(worldmap)
	dev.off()
}

