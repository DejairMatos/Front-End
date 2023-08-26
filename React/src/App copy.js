import './App.css';
import AnotherComponet from './components/AnotherComponent';
import FirstComponent from './components/FirstComponent';
import Hooks from './components/Hooks';
import Images from './components/Images';
import List from './components/List';
import RenderCond from './components/RenderCond';
import Fragment from './components/Fragment';
import Container from './components/Container';
import HttpRequestComponent from './components/HttpRequestComponent';
import ToggleComponent from './components/ToggleComponent';
import SliderComponent from './components/SliderComponent';
import ButtonLamp from './components/ButtonLamp';
import SliderLamp from './components/SliderLamp';

function App() {
  return (
    <div className="App">
      <h1>Hello World!</h1>
      <FirstComponent />
      <AnotherComponet />
      <Images />
      <Hooks />
      <List />
      <RenderCond x={8} y={10} />
      <Fragment />
      <Container>
        <h1>Este é filho do container!</h1>
      </Container>
      <hr />
      <HttpRequestComponent />
      <hr />
      <h1> Dashboard React</h1>
      <h2>Lâmpada</h2>
      <ButtonLamp />
      <SliderLamp />
      <br />
      <h2>Ventilador</h2>
      <ToggleComponent />
      <SliderComponent />
    </div>
  );
}

export default App;
