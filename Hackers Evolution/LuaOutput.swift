//
//  LuaOutput.swift
//  Hackers Evolution
//
//  Created by Eric Baur on 4/26/25.
//

import SwiftUI
import SwiftyLua

struct LuaOutput : View {
    let script: String
    var output: [String] = []
    
    
    init(script: String) {
        let lua = LuaVM()
        self.script = script
        
        let result = try? lua.execute(string: script)

        if let result {
            if case VirtualMachine.EvalResults.values(let returnValue) = result {
                self.output = returnValue.map{ "\($0.self)" }
            } else if case VirtualMachine.EvalResults.error(let error) = result {
                self.output = ["error: \(error)"]
            } else {
                self.output = ["result valid, no value"]
            }
        } else {
            self.output = ["result not valid"]
        }
    }
    
    var body: some View {
        List(output, id: \.self) {
            Text($0)
        }
    }
}

#Preview("Non-trivial script") {
    LuaOutput(script: """
          function fib(m)
            if m < 2 then
              return m
            end
            return fib(m-1) + fib(m-2)
          end

          return fib(15)
          """)
}

#Preview("Multi-output") {
    LuaOutput(script: "return 1, 'two', 3, 4, 'five'")
}

// not sure how to trigger the error clause
//#Preview("Errors") {
//    LuaOutput(script: "return 0/0")
//}

#Preview("Invalid") {
    LuaOutput(script: "not a valid script")
}
