import React, { useState } from "react";
import axios from "axios";

const HttpRequestComponent = () => {
  const [response, setResponse] = useState(null);

  const handleClick = async () => {
    try {
      const url = "http://dejairmatos.duckdns.org:1880/Teste";
      const content = { Teste: "Hello" }; //"Hello";

      const response = await axios.post(url, content);

      setResponse(response.data);
    } catch (error) {
      console.error("Error:", error);
    }
  };

  return (
    <div>
      <button onClick={handleClick}>Fazer Requisição</button>
      <div>
        <p>Resposta da Requisição: {JSON.stringify(response, null, 2)}</p>
      </div>
    </div>
  );
};

export default HttpRequestComponent;
