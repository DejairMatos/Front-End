import './App.css';
import ButtonLamp from './components/ButtonLamp';
import SliderLamp from './components/SliderLamp';
import SliderVent from './components/SliderVent';
import ButtonVent from './components/ButtonVent';
import HttpRequestComponent from './components/HttpRequestComponent';


function App() {
  return (
    <div className="App">
      <h1> Dashboard React</h1>
      <h2>Lâmpada</h2>
      <ButtonLamp />
      <SliderLamp />
      <br />
      <h2>Ventilador</h2>
      <ButtonVent />
      <SliderVent />
      <br />
      <h2>Temperatura / Humidade</h2>
      <HttpRequestComponent />
    </div>
  );
}

export default App;
