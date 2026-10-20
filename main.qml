import QtGraphs
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: win
    visible: true
    visibility: Window.FullScreen
    //flags: Qt.Window | Qt.FramelessWindowHint
    title: "Temperature"

	Item {    
	    width: 450
	    height: 400
	    anchors.centerIn: parent
	    Column {
		    anchors.centerIn: parent
		    spacing: 0
		    width: parent.width
			height: parent.height
			
		    Connections {
		        target: model
			}
			Row {
	    		width: parent.width
	    		height: 20
	    		Text {
	    			width: parent.width * 0.5
	    			height: parent.height
	    			font.family: "Ubuntu"
	    			font.pointSize: 10
	    			horizontalAlignment: Text.AlignLeft
	 			   	text: {
			        	const d = new Date(model.t0Timestamp)
			        	return Qt.formatDateTime(d, "ddd MMM yyyy hh:mm:ss ttt")
			        }
				}
	    		Text {
	    			width: parent.width * 0.5
	    			height: parent.height
	    			font.family: "Ubuntu"
	    			horizontalAlignment: Text.AlignRight
	    			font.pointSize: 10
	    			text: {
			        	const d = new Date(model.timestamp)
			        	return Qt.formatDateTime(d, "ddd MMM yyyy hh:mm:ss")
			        }
			        color:"green"
			    }
			}
			        
	    	Row {
	    		//anchors.centerIn: parent
	    		spacing: 0
	    		width: parent.width
	    		height: parent.height * 0.5
	    		Column {
	    			spacing: 0    			
					width: parent.width * 0.2
					height: parent.height
				    Rectangle {
				    	width: parent.width
						height: 20	    
					    color: "#b0e0e6"
						Text {
		    				height: parent.height * 0.2
					        id: max_temperature
					        text: "" + model.maxTemperature.toFixed(2) + " °C"
					        color: model.maxTemperature > 30  ? "red" : model.maxTemperature < 5 ? "lightsteelblue" : "black"
					        font.pointSize: 10
					        font.family: "Ubuntu"
					        horizontalAlignment: Text.AlignHCenter
					        //anchors.horizontalCenter: parent.horizontalCenter
					    }
					}
				    Rectangle {
				    	width: parent.width
						height: 20
					    color: "#b0e0e6"
						Text {
		    				height: parent.height * 0.2
					        id: min_temperature
					        text: "" + model.minTemperature.toFixed(2) + " °C"
					        color: model.minTemperature > 30  ? "red" : model.minTemperature < 5 ? "lightsteelblue" : "black"
					        font.pointSize: 10
					        font.family: "Ubuntu"
					        horizontalAlignment: Text.AlignHCenter
					        //anchors.horizontalCenter: parent.horizontalCenter
					    }
					}
					Rectangle {
				    	width: parent.width
						height: parent.height    
					    color: "#b0e0e6"
					}
	    		}
			    Rectangle {
			    	width: parent.width * 0.8
					height: parent.height		    
				    color: "#b0e0e6"
				    Text {
				    	width: parent.width 
				    	height: parent.height
				        id: temperature
				        text: "" + model.temperature.toFixed(1) + " °C"
				        color: model.temperature > 30  ? "red" : model.temperature < 5 ? "lightsteelblue" : "black"
				        font.pointSize: 60
				        font.family: "Ubuntu"
				        horizontalAlignment: Text.AlignHCenter
				        verticalAlignment: Text.AlignVCenter
				        //anchors.horizontalCenter: parent.horizontalCenter
				        //backgroudColor: #b0e0e6
				    }
				}
	        }
	    
	        id : root
	
			function xAxisSize() {
				let k = 100
				let e = (model.timestamp - model.t0Timestamp) / 1000
				while (e + 10 > k) {
					k += 100
				}
				console.log("e=", e, ", k=",k)
				return k 
			}
	
	//		function xAxisMin() {
	//			//console.log("min=", model.temperatures.count)
	//			return model.temperatures.count == 0 ? 0 : model.temperatures.at(0).x 
	//		}
				
	//		Rectangle {
	//			id: rect
	//			width: parent.width
	//			height: 160//parent.height
	//			color: "#00e0e6"
			
			Row {
				width: parent.width 
				height: parent.height * 0.4
				
				GraphsView {
			        //anchors.fill: parent
			        id:rect
			        theme: GraphsTheme {
				        theme: GraphsTheme.Theme.UserDefined
				        backgroundColor: "#b0e0e6"
				        backgroundVisible: true
				        seriesColors: ["green"]
				        colorStyle: GraphsTheme.ColorStyle.Uniform
				        //labelFont.family: "Lucida Handwriting"
				        labelFont.family: "Ubuntu"
				        labelFont.pointSize: 25
				        gridVisible: false
				        grid.mainColor: "red"
				        grid.subColor: "blue"
				        labelBackgroundColor: "black"
				        labelBackgroundVisible: false
				        labelBorderVisible: false
				        labelTextColor: "white"
				        multiHighlightColor: "green"
				        singleHighlightColor: "darkRed"
				        plotAreaBackgroundVisible: false
				        labelsVisible: false
				        axisX.mainColor: "#ccccff"
				    }
			
					width: parent.width 
					height: parent.height
					marginLeft: 5
					marginTop: 0
					marginRight: 20
					marginBottom: 0
					
			        
			        axisX: ValueAxis {
			            min: model.temperatures.count == 0 ? 0 : model.temperatures.at(0).x 
			            max: root.xAxisSize()
			            tickInterval: 20
			            visible: false
			            lineVisible: false
			        }
			
			        axisY: ValueAxis {
			            min: model.minTemperature - 5
			            max: model.maxTemperature + 5
			            visible: false
			            //labelsVisible: true
			            lineVisible: false
		    			gridVisible: false
			            tickAnchor: min
		    			tickInterval: max - min
		    			//color: "#b0e0e6" no color on 6.10 use >= 6.11
		    			titleVisible: false
			        }
			
					Component.onCompleted: {
						addSeries(model.temperatures)
						//console.log("Loaded")	
						//root.xAxisMin()	
					}				
				}			
			}
	
		    Row {
	    		width: parent.width
	    		height: 20
	    		Text {
	    			width: parent.width 
	    			height: parent.height
	    			horizontalAlignment: Text.AlignLeft
	    			font.pointSize: 10
	    			font.family: "Ubuntu"
	 			   	text: model.serviceAvailable ? "online" : "offline"
	 			   	color: model.serviceAvailable ? "green" : "red"
				}
			}		
		}
	}	
	Component.onCompleted: {
        console.log("Screen:", Screen.width, "x", Screen.height,
                    "DPR:", Screen.devicePixelRatio)
    }
}