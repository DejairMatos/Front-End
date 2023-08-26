import React, { useState, useEffect } from "react";
import axios from "axios";

const SliderVent = () => {
  const [sliderValue, setSliderValue] = useState(0);
  const [response, setResponse] = useState(null);
  const [timerId, setTimerId] = useState(null);

  // Manipula a mudança do slider
  const handleSliderChange = (event) => {
    const newValue = parseInt(event.target.value, 10);
    setSliderValue(newValue);

    // Reinicia o temporizador sempre que o slider for movido
    clearTimeout(timerId);
  };

  // Manipula o evento de soltar o botão do mouse após mover o slider
  const handleSliderRelease = async () => {
    try {
      const url = "http://dejairmatos.duckdns.org:1880/SliderVent";
      const content = { slider: sliderValue };

      const response = await axios.post(url, content);

      setResponse(response.data);
    } catch (error) {
      console.error("Error:", error);
    }
  };

  // Efeito que é executado quando o valor do slider é alterado
  useEffect(() => {
    // Define um novo temporizador para enviar após 1 segundo de inatividade
    const newTimerId = setTimeout(() => {
      handleSliderRelease();
    }, 1000);

    setTimerId(newTimerId);

    // Limpa o temporizador ao desmontar o componente ou quando o valor do slider é alterado novamente
    return () => {
      clearTimeout(newTimerId);
    };
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [sliderValue]);

  return (
    <div>
      <div>
        {/* Slider para selecionar um valor entre 0 e 100 */}
        <input
          type="range"
          min={0}
          max={100}
          value={sliderValue}
          onChange={handleSliderChange}
          onMouseUp={handleSliderRelease}
        />
      </div>
      <div>
        {/* Mostra o valor atual do slider */}
        <p>Valor do Slider: {sliderValue}</p>
        {/* Mostra a resposta da requisição */}
        <p>Resposta da Requisição: {JSON.stringify(response, null, 2)}</p>
      </div>
    </div>
  );
};

export default SliderVent;
