// import React, { useState, useEffect } from "react";
// import axios from "axios";

// const DataShow = () => {
//   const [logs, setLogs] = useState({});
//   const [isMachineOn, setIsMachineOn] = useState([false, false, false, false]);
//   const [noOfOn, setNoOfOn] = useState(0);

//   const fetchLatestLogs = async () => {
//     try {
//       const response = await fetch("http://localhost:3000/latest-log");
//       if (response.ok) {
//         const data = await response.json();
//         setLogs(data); // Set logs as an object
//       } else {
//         console.log("No data available");
//       }
//     } catch (error) {
//       console.error("Error fetching logs:", error);
//     }
//   };

//   useEffect(() => {
//     const interval = setInterval(fetchLatestLogs, 1000);
//     return () => clearInterval(interval);
//   }, []);

//   useEffect(() => {
//     countOnMachines();
//   }, [isMachineOn]);

//   const renderMachineLogs = (machineId) => {
//     const machineLogs = logs[machineId] || []; // Access logs for the specific machine

//     return (
//       <div key={machineId}>
//         <h1 className="w-[45vw] px-4 text-xl font-semibold text-center text-blue-400">
//           Machine-{machineId}
//         </h1>
//         {machineLogs.length > 0 ? (
//           <div className="w-[52vw] p-4 flex justify-center rounded-lg">
//             <table className="w-[50vw] table-auto border-collapse text-left">
//               <thead>
//                 <tr className="text-blue-400 rounded-xl">
//                   <th className="px-4 py-2 border border-gray-700 text-center">Message</th>
//                   <th className="px-4 py-2 border border-gray-700 text-center">Date</th>
//                   <th className="px-4 py-2 border border-gray-700 text-center">Time</th>
//                 </tr>
//               </thead>
//               <tbody>
//                 {machineLogs.map((log, index) => (
//                   <tr key={`${machineId}-${index}`}>
//                     <td className="px-4 py-2 border border-gray-700 text-center">{log.message}</td>
//                     <td className="px-4 py-2 border border-gray-700 text-center">{log.date}</td>
//                     <td className="px-4 py-2 border border-gray-700 text-center">{log.time}</td>
//                   </tr>
//                 ))}
//               </tbody>
//             </table>
//           </div>
//         ) : (
//           <p className="text-center text-lg my-14">No logs available</p>
//         )}
//       </div>
//     );
//   };

//   const countOnMachines = () => {
//     const count = isMachineOn.filter(Boolean).length;
//     setNoOfOn(count);
//   };

//   const handleStateChangeButton = async (index) => {
//     const machineId = (index + 1).toString();
//     const task = isMachineOn[index] ? "stop" : "start";

//     try {
//       const response = await axios.post("http://localhost:3000/control-machine", {
//         machineId,
//         task,
//       });

//       if (response.data.success) {
//         setIsMachineOn((prevStates) => {
//           const newStates = [...prevStates];
//           newStates[index] = !newStates[index];
//           return newStates;
//         });
//         console.log(response.data.message);
//       }
//     } catch (error) {
//       console.error("Failed to send machine control command:", error);
//     }
//   };

//   const handleStartAll = async () => {
//     await Promise.all(
//       isMachineOn.map((state, index) => (!state ? handleStateChangeButton(index) : null))
//     );
//   };

//   const handleStopAll = async () => {
//     await Promise.all(
//       isMachineOn.map((state, index) => (state ? handleStateChangeButton(index) : null))
//     );
//   };

//   return (
//     <div className="min-h-screen bg-black text-white p-6">
//       <div className="border-blue-400 border-[1px] mt-6 rounded-xl p-6 flex flex-wrap h-max gap-10">
//         <div className="flex gap-14 border-[1px] border-gray-600 rounded-xl px-4 py-2">
//           <div className="flex flex-col justify-center items-center gap-2">
//             <p className="font-bold text-blue-400">Machine State Data</p>
//             <button className="border-[1px] border-white text-sm py-1 px-2 rounded-lg cursor-pointer hover:text-black hover:bg-white" onClick={handleStartAll}>
//               Start All
//             </button>
//             <button className="border-[1px] border-white text-sm py-1 px-2 rounded-lg cursor-pointer hover:text-black hover:bg-white" onClick={handleStopAll}>
//               Stop All
//             </button>
//           </div>
//           <div className="flex flex-col justify-center items-center gap-1">
//             <p>Total machines: {isMachineOn.length}</p>
//             <p>Total on machines: {noOfOn}</p>
//             <p>Total off machines: {isMachineOn.length - noOfOn}</p>
//           </div>
//         </div>
//         <div className="flex gap-20">
//           {isMachineOn.map((state, index) => (
//             <div
//               key={index}
//               className="border-[1px] border-gray-600 rounded-xl flex flex-col px-10 justify-center items-center gap-4"
//             >
//               <p className="font-bold text-blue-400">Machine - {index + 1}</p>
//               <button
//                 className="border-[1px] border-white text-sm py-1 px-2 rounded-lg cursor-pointer hover:text-black hover:bg-white"
//                 onClick={() => handleStateChangeButton(index)}
//               >
//                 {state ? "Turn Off" : "Turn On"}
//               </button>
//             </div>
//           ))}
//         </div>
//       </div>
//       <div className="border-blue-400 border-[1px] mt-6 rounded-xl p-6 flex flex-wrap h-max">
//         {[1, 2, 3, 4].map((machineId) => renderMachineLogs(machineId))}
//       </div>
//     </div>
//   );
// };

// export default DataShow;



import React, { useState, useEffect } from "react";
import axios from "axios";

const DataShow = () => {
  const [logs, setLogs] = useState({});
  const [isMachineOn, setIsMachineOn] = useState([false, false, false, false]);
  const [noOfOn, setNoOfOn] = useState(0);

  const fetchLatestLogs = async () => {
    try {
      const response = await fetch("http://localhost:3000/latest-log");
      if (response.ok) {
        const data = await response.json();
        setLogs(data); // Set logs as an object
      } else {
        console.log("No data available");
      }
    } catch (error) {
      console.error("Error fetching logs:", error);
    }
  };

  useEffect(() => {
    const interval = setInterval(fetchLatestLogs, 1000);
    return () => clearInterval(interval);
  }, []);

  useEffect(() => {
    countOnMachines();
  }, [isMachineOn]);

  const renderMachineLogs = (machineId) => {
    const machineLogs = logs[machineId] || []; // Access logs for the specific machine

    return (
      <div key={machineId} className="mb-8">
        <h1 className="w-[45vw] px-4 text-xl font-semibold text-center text-blue-400">
          Machine-{machineId}
        </h1>
        {machineLogs.length > 0 ? (
          <div className="w-[52vw] p-4 flex justify-center rounded-lg">
            <div className="w-[50vw] max-h-[300px] overflow-y-auto">
              <table className="w-full table-auto border-collapse text-left">
                <thead>
                  <tr className="text-blue-400 rounded-xl">
                    <th className="px-4 py-2 border border-gray-700 text-center">Message</th>
                    <th className="px-4 py-2 border border-gray-700 text-center">Date</th>
                    <th className="px-4 py-2 border border-gray-700 text-center">Time</th>
                  </tr>
                </thead>
                <tbody>
                  {machineLogs.map((log, index) => (
                    <tr key={`${machineId}-${index}`}>
                      <td className="px-4 py-2 border border-gray-700 text-center">{log.message}</td>
                      <td className="px-4 py-2 border border-gray-700 text-center">{log.date}</td>
                      <td className="px-4 py-2 border border-gray-700 text-center">{log.time}</td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </div>
        ) : (
          <p className="text-center text-lg my-14">No logs available</p>
        )}
      </div>
    );
  };

  const countOnMachines = () => {
    const count = isMachineOn.filter(Boolean).length;
    setNoOfOn(count);
  };

  const handleStateChangeButton = async (index) => {
    const machineId = (index + 1).toString();
    const task = isMachineOn[index] ? "stop" : "start";

    try {
      const response = await axios.post("http://localhost:3000/control-machine", {
        machineId,
        task,
      });

      if (response.data.success) {
        setIsMachineOn((prevStates) => {
          const newStates = [...prevStates];
          newStates[index] = !newStates[index];
          return newStates;
        });
        console.log(response.data.message);
      }
    } catch (error) {
      console.error("Failed to send machine control command:", error);
    }
  };

  const handleStartAll = async () => {
    await Promise.all(
      isMachineOn.map((state, index) => (!state ? handleStateChangeButton(index) : null))
    );
  };

  const handleStopAll = async () => {
    await Promise.all(
      isMachineOn.map((state, index) => (state ? handleStateChangeButton(index) : null))
    );
  };

  return (
    <div className="min-h-screen bg-black text-white p-6">
      <div className="border-blue-400 border-[1px] mt-6 rounded-xl p-6 flex flex-wrap h-max gap-10">
        <div className="flex gap-14 border-[1px] border-gray-600 rounded-xl px-4 py-2">
          <div className="flex flex-col justify-center items-center gap-2">
            <p className="font-bold text-blue-400">Machine State Data</p>
            <button className="border-[1px] border-white text-sm py-1 px-2 rounded-lg cursor-pointer hover:text-black hover:bg-white" onClick={handleStartAll}>
              Start All
            </button>
            <button className="border-[1px] border-white text-sm py-1 px-2 rounded-lg cursor-pointer hover:text-black hover:bg-white" onClick={handleStopAll}>
              Stop All
            </button>
          </div>
          <div className="flex flex-col justify-center items-center gap-1">
            <p>Total machines: {isMachineOn.length}</p>
            <p>Total on machines: {noOfOn}</p>
            <p>Total off machines: {isMachineOn.length - noOfOn}</p>
          </div>
        </div>
        <div className="flex gap-20">
          {isMachineOn.map((state, index) => (
            <div
              key={index}
              className="border-[1px] border-gray-600 rounded-xl flex flex-col px-10 justify-center items-center gap-4"
            >
              <p className="font-bold text-blue-400">Machine - {index + 1}</p>
              <button
                className="border-[1px] border-white text-sm py-1 px-2 rounded-lg cursor-pointer hover:text-black hover:bg-white"
                onClick={() => handleStateChangeButton(index)}
              >
                {state ? "Turn Off" : "Turn On"}
              </button>
            </div>
          ))}
        </div>
      </div>
      <div className="border-blue-400 border-[1px] mt-6 rounded-xl p-6 flex flex-wrap h-max">
        {[1, 2, 3, 4].map((machineId) => renderMachineLogs(machineId))}
      </div>
    </div>
  );
};

export default DataShow;