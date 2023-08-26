import React, { useState } from "react";
import axios from "axios";

const ButtonLamp = () => {
  const [status, setStatus] = useState("OFF");
  // eslint-disable-next-line no-unused-vars
  const [response, setResponse] = useState(null);

  const handleClick = async () => {
    try {
      const url = "http://dejairmatos.duckdns.org:1880/ButtonLamp";
      const newStatus = status === "OFF" ? "ON" : "OFF";
      const content = { status: newStatus };

      const response = await axios.post(url, content);

      setStatus(newStatus);
      setResponse(response.data);
    } catch (error) {
      console.error("Error:", error);
    }
  };

  return (
    <div>
      <button onClick={handleClick}>
        {status === "OFF" ? "OFF" : " ON "}
      </button> 
      <div>
        <p>Status Atual: {status}</p>
        <p>Resposta da API: {JSON.stringify(response, null, 2)}</p>
      </div>
    </div>
  );
};

export default ButtonLamp;
