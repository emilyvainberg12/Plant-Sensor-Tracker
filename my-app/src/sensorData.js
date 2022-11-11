function sensorData () {
    const SERVERADDRESS = "http://127.0.0.1:3000"
    // use XML HTTP request to grab data from the server (central)
    let xmlhttp = new XMLHttpRequest()
    var dataset1 = [
    ];

    var startTime = new Date();
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState === 4 && xmlhttp.status === 200) {
            let responseText = xmlhttp.responseText

            let sensorValue = parseFloat(JSON.parse(responseText).sensorValue)

            var endTime = new Date();
            var timeDiff = endTime - startTime; 
            timeDiff /= 1000;

            dataset1.push([timeDiff, sensorValue])
            if (timeDiff > 10){
                dataset1 = []
                startTime = new Date()
            }
    }
    // periodically grab data
    let getData = () => {
        xmlhttp.open("POST", SERVERADDRESS, true)
        xmlhttp.setRequestHeader("Content-type", "application/json")
        xmlhttp.send("")
        setTimeout(() => {
            getData()
        }, 10)
    }
    getData();
    }
}
export default sensorData;