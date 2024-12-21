// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const { SerialPort } = require('serialport')
const { ReadlineParser } = require('@serialport/parser-readline')
const tableify = require('tableify')

//lets create array which simulate queue of incoming data (up to five elements)
let tempQueue = ['','','','','']
let pressureQueue = ['','','','','']
//lets create five rows in the console
let rows = ['empty','empty','empty','empty','empty']
//lets create date some hours ago to simulate last time of incoming data
let lastTimeOfIncomingTemperature = new Date() - 1000*60*60*2
let lastTimeOfIncomingPressure = new Date() - 1000*60*60*2
let lastIncomingData = ""

const dateDifferenceInSeconds = (date1, date2) => {
  return Math.abs(date1 - date2) / 1000
}
//function gets date object and create time string in format HH:MM:SS
const createTimeString = (date) => {
  //add leading zero to hours, minutes and seconds if they are less than 10
  const hours = date.getHours() < 10 ? '0' + date.getHours() : date.getHours()
  const minutes = date.getMinutes() < 10 ? '0' + date.getMinutes() : date.getMinutes()
  const seconds = date.getSeconds() < 10 ? '0' + date.getSeconds() : date.getSeconds()
  return `${hours}:${minutes}:${seconds}`
}

//connect to the serial port

const port = new SerialPort({
    path: '/dev/ttyACM0',
    baudRate: 9600
    }, function(err) {
        if(err) {
            return console.log('Error: ', err.message)
        };
    console.log('connected to port')
}
);

// port.on('data', function(data) {
//     console.log('Data:', data)
// });


const parser = port.pipe(new ReadlineParser())
parser.on('data', (line) => {
    //# - txt message
    //$ - temperature message
    //&-pressure message
    //console.log('got line', line)
    // check if incoming data is not a duplicate of last incoming data
    if(line === lastIncomingData) {
      return
    } else {
      lastIncomingData = line
    }
    console.log('got line', line)
    if(line[0] === '#') {
      document.getElementById('txtmsg').textContent = line.slice(1)
    }else if (line[0] ==='$') {
      //remove first character from line
      const timeOfIncoming = new Date()
      if(dateDifferenceInSeconds(timeOfIncoming, lastTimeOfIncomingTemperature) > 1) {
        line = line.slice(1)
        tempQueue.shift()
        tempQueue.push(line)
        document.getElementById('temperatureoutfirst').textContent = tempQueue[4]
        document.getElementById('temperatureoutothers').textContent = tempQueue[3] + ' ' + tempQueue[2] + ' ' + tempQueue[1] + ' ' + tempQueue[0]
        lastTimeOfIncomingTemperature = timeOfIncoming
        console.log(tempQueue)
      }
    } else if(line[0] === '&') {
      //remove first character from line
      const timeOfIncoming = new Date()
      if(dateDifferenceInSeconds(timeOfIncoming, lastTimeOfIncomingPressure) > 1) {
        line = line.slice(1)
        pressureQueue.shift()
        pressureQueue.push(line)
        document.getElementById('pressurefirst').textContent = pressureQueue[4]
        document.getElementById('pressureothers').textContent = pressureQueue[3] + ' ' + pressureQueue[2] + ' ' + pressureQueue[1] + ' ' + pressureQueue[0]
        lastTimeOfIncomingPressure = timeOfIncoming
        console.log(pressureQueue)
      }
    } else {
      //document.getElementById('msg').textContent = line
      //lets create a string with hour minute and seconds of incoming data
      const timeOfIncoming = new Date()
      const timeString = createTimeString(timeOfIncoming)
      //add timeString to the beginning of the line
      line = `${timeString}:  ${line}`
      rows.shift()
      rows.push(line)
      for(let [index, row] of rows.entries()) {
        document.getElementById('row-' + index).textContent = row
      }
    }
});


async function listSerialPorts() {
  await SerialPort.list().then((ports, err) => {
    if(err) {
      document.getElementById('error').textContent = err.message
      return
    } else {
      document.getElementById('error').textContent = ''
    }
    console.log('ports', ports);

    if (ports.length === 0) {
      document.getElementById('error').textContent = 'No ports discovered'
    }

    tableHTML = tableify(ports)
    document.getElementById('ports').innerHTML = tableHTML
  })
}

function listPorts() {
  listSerialPorts();
  setTimeout(listPorts, 2000);
}

const btnON = document.getElementById('sendon');
btnON.addEventListener('click', function() {
    port.write('ON')
})
const btnOFF = document.getElementById('sendoff');
btnOFF.addEventListener('click', function() {
    port.write('OFF')
})


// Set a timeout that will check for new serialPorts every 2 seconds.
// This timeout reschedules itself.
// setTimeout(listPorts, 2000);

//listSerialPorts()