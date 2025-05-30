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
    
    let lua = LuaVM()
    
    func loadScript(file: String, global: String) {
        guard let url = Bundle.main.url(forResource: file, withExtension: "lua") else {
            fatalError("\(file).lua not found")
        }
      
        let intermediateResult = try? lua.execute(url: url)

        if let intermediateResult {
            if case VirtualMachine.EvalResults.values(let returnValue) = intermediateResult {
                lua.globals[global] = returnValue[0]
            }
        } else {
            print("failed to incorporate intermediate result")
        }
    }
    
    init(script: String) {
        self.script = script
        
        loadScript(file: "tape", global: "Tape")
        loadScript(file: "hela", global: "HELa")
        
        let result = try? lua.execute(string: script)

        if let result {
            if case VirtualMachine.EvalResults.values(let returnValue) = result {
                
                self.output = returnValue.flatMap { value in
                    return inspect(value: value)
                }
            } else if case VirtualMachine.EvalResults.error(let error) = result {
                self.output = ["error: \(error)"]
            } else {
                self.output = ["result valid, no value"]
            }
        } else {
            self.output = ["result not valid"]
        }
    }
    
    func inspect(value: lua4swift.Value, key: String? = nil) -> [String] {
        if let table = value as? lua4swift.Table {
            return inspect(table: table, key: key)
        } else if let other = value as? lua4swift.Userdata {
            print("other?")
            return ["?\(other)?"]
        } else {
            if let key {
                return ["\(key) = \(value.self)"]
            } else {
                return ["\(value.self)"]
            }
        }
    }
    
    func inspect(table: lua4swift.Table, key: String? = nil) -> [String] {
        return table.keys().flatMap { subkey in
            if let key {
                // Okay, this will do weird things with complex keys
                // can lua do that?
                return inspect(value:table[subkey], key: "\(key).\(subkey)")
            } else {
                return inspect(value:table[subkey], key:"\(subkey)")
            }
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

#Preview("Heirarchical output") {
    LuaOutput(script: "return he.state, 'foo', { one=1, two={ three='foo', four={ six='bar', 7 } }}, { 1, 2, 3, 4 }")
}

#Preview("Function output") {
    LuaOutput(script: "return { aFunc = function () return 'foo'; end}")
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
