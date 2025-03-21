import React from 'react'

const Header = () => {
  return (
    <div className='w-full h-[10vh] bg-neutral-900 flex justify-between items-center lg:px-6 border-b-[1px] border-gray-600'>
      <h1 className='text-4xl font-extrabold text-blue-400'>Nessco</h1>
      <div className='text-gray-50 list-none flex gap-8'>
        <li className='hover:text-blue-400 text-white cursor-pointer'>Analytics</li>
        <li className='hover:text-blue-400 text-white cursor-pointer'>Records</li>
        <li className='hover:text-blue-400 text-white cursor-pointer'>Contact</li>
      </div>
      <div className='flex text-gray-50 gap-8'>
        <button className='border-[1px] border-white px-4 py-2 rounded-lg cursor-pointer hover:text-black hover:bg-white font-bold'>Login</button>
        <button className='border-[1px] border-white px-4 py-2 rounded-lg cursor-pointer hover:text-black hover:bg-white font-bold'>Signup</button>
      </div>
    </div>
  )
}

export default Header
