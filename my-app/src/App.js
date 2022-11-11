import './css/App.css';
import Pong from 'react-pong';
// import sensorData from './sensorData'

function App() {
  return (
    <div className="App">
      <h1> Welcome to Your Personal Plant Tracker</h1>
      <p> Choose Plans to Track: </p>
      <div className='GetSensorData'>
        <p> Sensor Data: </p>
        <Pong></Pong>
        {/* React.render(
          <Pong/>,
          document.getElementById('container')
        ); */}
        {/* <sensorData></sensorData> */}
      </div>
    </div>
  );
}

export default App;
