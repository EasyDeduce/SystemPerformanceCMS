# Load the necessary libraries
library(ggplot2)

# Load data
data <- read.csv("C:/Users/Saksham Rao/Desktop/CAO PROJECT/CAO CMS/output/performance_data.csv", check.names=FALSE)

# Check the structure of the data
str(data)

# Remove rows with NA values
data <- na.omit(data)

# Ensure Time column is in the correct format (if needed)
data$Time <- as.numeric(data$Time)  # Ensure Time is numeric

# Create a PNG file for output
png("C:/Users/Saksham Rao/Desktop/performance_plots.png", width=1200, height=800)

# Set up 2x2 plotting area
par(mfrow=c(2,2))

# Plot 1: Cache Hits and Misses
if (nrow(data) > 0) {
    plot(data$Time, data$Cache.Hits, type="l", col="blue",
         main="Cache Hits and Misses Over Time",
         xlab="Time", ylab="Count",
         lwd=2)
    lines(data$Time, data$Cache.Misses, col="red", lwd=2)
    legend("topright", legend=c("Hits", "Misses"),
           col=c("blue", "red"), lty=1, lwd=2)
} else {
    message("No data available for plotting Cache Hits and Misses.")
}

# Plot 2: Hit Rate
if (nrow(data) > 0) {
    plot(data$Time, data$Hit.Rate, type="l", col="green",
         main="Hit Rate Over Time",
         xlab="Time", ylab="Hit Rate (%)",
         lwd=2)
} else {
    message("No data available for plotting Hit Rate.")
}

# Plot 3: Average Access Time
if (nrow(data) > 0) {
    plot(data$Time, data$Average.Access.Time, type="l", col="purple",
         main="Average Access Time",
         xlab="Time", ylab="Access Time",
         lwd=2)
} else {
    message("No data available for plotting Average Access Time.")
}

# Plot 4: CPU and Memory Utilization
if (nrow(data) > 0) {
    plot(data$Time, data$CPU.Utilization, type="l", col="orange",
         main="CPU and Memory Utilization",
         xlab="Time", ylab="Utilization (%)",
         lwd=2)
    lines(data$Time, data$Memory.Utilization, col="brown", lwd=2)
   